#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	This is the class for and object in the game						//
//	An object can OR NOT have a model. If it does, it is loaded			//
//	Every object has a NAME, it will be useful to search through them.	//
//	Also, every object has some variables such as position, rotation,	//
//	etc etc ...															//
//																		//
// --------------------------------------------------------------------	//

/*
**	The constructor for an object without a model.
**	We can imagine the camera for example, or a simple point in world space.
*/

GameObject::GameObject(std::string objName)
{
	HasModel = false;
	HasTexture = false;
	_objTexture = NULL;
	ObjPath = "";
	Name = objName;
	initValues();

	// Add this object to the engine's list of objects.
	GameEngineController::Instance().GameObjectList.push_back(this);
}

/*
**	The constructor for an object with a model.
**	This will load the buffers required, and allow this object
**	to be "drawn" by the engine.
*/

GameObject::GameObject(std::string objName, std::string path)
{
	FILE * file;

	file = fopen(path.c_str(), "r");
	if (file == NULL)
	{
		std::cout << "Impossible to open the file !" << std::endl;
		return ;
	}
	Name = objName;
	ObjPath = path;
	HasModel = true;
	HasTexture = false;
	_objTexture = NULL;
	// set initial values to zero.
	initValues();
	// Parse the obj file to scoop values.
	getObjValues(file);
	// set bounding box center -> object math center.
	BoundingBoxCenter.x = ((BoundingBoxMin.x + BoundingBoxMax.x) / 2.0);
	BoundingBoxCenter.y = ((BoundingBoxMin.y + BoundingBoxMax.y) / 2.0);
	BoundingBoxCenter.z = ((BoundingBoxMin.z + BoundingBoxMax.z) / 2.0);


	// // ----- PRINT Bounding box
	// std::cout << "min = " << BoundingBoxMin.x << "x "
	// 			<< BoundingBoxMin.y << "y "
	// 			<< BoundingBoxMin.z << "z" << std::endl;

	// std::cout << "center = " << BoundingBoxCenter.x << "x "
	// 			<< BoundingBoxCenter.y << "y "
	// 			<< BoundingBoxCenter.z << "z" << std::endl;

	// std::cout << "max = " << BoundingBoxMax.x << "x "
	// 			<< BoundingBoxMax.y << "y "
	// 			<< BoundingBoxMax.z << "z" << std::endl << std::endl;

	// create faces from indexes.
	createObjFaces();
	// set opengl buffers
	setBuffers();
	// load texture.
	loadTexture();
	// Add this object to the engine's list of objects.
	GameEngineController::Instance().GameObjectList.push_back(this);
}

void		GameObject::initValues()
{
	Position = glm::vec3(0.0, 0.0, 0.0);
	// for now, we will imagine euleur rotations.
	Rotation = glm::vec3(0.0, 0.0, 0.0);
	Scale = glm::vec3(1.0, 1.0, 1.0);

	BoundingBoxMin = glm::vec3(0.0, 0.0, 0.0);
	BoundingBoxMax = glm::vec3(0.0, 0.0, 0.0);
	BoundingBoxCenter = glm::vec3(0.0, 0.0, 0.0);
}



GameObject::~GameObject()
{

}

void	GameObject::createObjFaces()
{
	int i = 0;

	while (i != (int)_objVertexIndices.size())
	{
		_objFacesVertices.push_back(_objVertices[_objVertexIndices[i] - 1]);
		i++;
	}
	i = 0;
	while (i != (int)_objUVIndices.size())
	{
		_objFacesUVs.push_back(_objUVs[_objUVIndices[i] - 1]);
		i++;
	}
	// ------ Set nb of values variables for quick access.
	_nbFaceVertices = _objFacesVertices.size();
	_nbFaces = _nbFaceVertices / 3;
}

/*
**	A note on the buffers. I am not withouth knowing of the elements buffer,
**	that could take my vertice list and use their index for drawing triangles quickly.
**	However, I went with another way for listing my faces, that is, I created a buffer
**	with every vertex for every face, arranged three by three.
**
**	That means YES, THERE IS OVERHEAD on the vertex loading, as a vertex can
**	be copied several time into the buffer.
**	I just prefer this way of loading the models as I am not limited in the number
**	of buffers, as it is the case with the element array.
*/

void		GameObject::setBuffers()
{
	// -------------------------------------------------------------------------- //
	//	VAO - Vertex Array object												  //
	// -------------------------------------------------------------------------- //
	_vao = 0;
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// -------------------------------------------------------------------------- //
	//	VBOs - Vertex buffer object												  //
	// -------------------------------------------------------------------------- //

	// generating buffer of face vertices -> the one used to draw.
	_fvbo = 0;
	glGenBuffers(1, &(_fvbo));

	glBindBuffer(GL_ARRAY_BUFFER, _fvbo);
	glBufferData(GL_ARRAY_BUFFER, _objFacesVertices.size() * sizeof(glm::vec4), &_objFacesVertices[0],
					GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// generating vbo buffers -> useless in truth, but we never know.
	// std::cout << "vertices nb : " << _objVertices.size() << std::endl;
	_vbo = 0;
	glGenBuffers(1, &(_vbo));

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _objVertices.size() * sizeof(glm::vec4), &_objVertices[0],
					GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	// for the normals
	_nbo = 0;
	glGenBuffers(1, &(_nbo));

	glBindBuffer(GL_ARRAY_BUFFER, _nbo);
	glBufferData(GL_ARRAY_BUFFER, _objNormals.size() * sizeof(glm::vec3), &_objNormals[0],
					GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	
	// the color buffer cbo. // UNUSED, as it is mainly made for randomly coloring faces.
	// sc->cbo = 0;
	// glGenBuffers(1, &(_cbo));

	// glBindBuffer(GL_ARRAY_BUFFER, _cbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((sc->total_faces * 3) * 3), sc->vertex_color_values, GL_STATIC_DRAW);
	// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// glEnableVertexAttribArray(2);

	// the faces uv buffer. -> stocking the uvs not coming from the "f" header has no utility.
	_fubo = 0;
	glGenBuffers(1, &(_fubo));

	glBindBuffer(GL_ARRAY_BUFFER, _fubo);
	glBufferData(GL_ARRAY_BUFFER, _objFacesUVs.size() * sizeof(glm::vec2), &_objFacesUVs[0],
					GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
}

/*
**	Once the buffer and faces are set, ill load the texture in memory.
**	Ill assume the texture is in the same directory as the .obj, with the same
**	name, and with the .bmp extension.
*/

void		GameObject::loadTexture()
{
	std::string texPath = ObjPath;
	texPath.resize(ObjPath.size() - 4);
	texPath.append(".bmp", 4);
	_objTexture = (t_bmp_texture *)malloc(sizeof(t_bmp_texture));
	if ((GameEngineController::LoadTextureFile(_objTexture, texPath)) == -1)
	{
		HasTexture = false;
		free(_objTexture);
		_objTexture = NULL;
		return ;
	}
	// the loaded texture is now in this->_objTexture
	HasTexture = true;
	glGenTextures(1, &_ObjTextureID);
}

/*
**	If an object is already textured, you can load a new texture with this method.
**	However, this is not the recommended method if you want to change texture many
**	times during runtime, as it will open and parse the texture file each time
**	it is called. This one is to be used casually.
**
**	For runtime multi texture buffering, use the SetTexture() public method, and load
**	bmp textures objects in your game code.
*/

void		GameObject::LoadNewTexture(std::string path)
{
	if (HasTexture == true)
	{
		glDeleteTextures(1, &_ObjTextureID);
		free(_objTexture->data);
		free(_objTexture);
		HasTexture = false;
	}
	if ((GameEngineController::LoadTextureFile(_objTexture, path)) == -1)
	{
		return ;
	}
	// the loaded texture is now in this->_objTexture
	HasTexture = true;
	glGenTextures(1, &_ObjTextureID);
}


// --------------------------------------------------------------------	//
//	GameObject's accessors												//
// --------------------------------------------------------------------	//

/*
**	Returns the object's vertex array object.
*/

GLuint		GameObject::GetVao()
{
	return (_vao);
}

/*
**	Returns the object's number of vertex for each triangle.
*/

int		GameObject::GetNbFaceVertices()
{
	return (_nbFaceVertices);
}

/*
**	Returns the object's number of face triangle.
*/

int		GameObject::GetNbFaces()
{
	return (_nbFaces);
}

/*
**	Returns the object's face vertex buffer object.
**	This is the one used for drawing.
*/

GLuint		GameObject::GetFvbo()
{
	return (_fvbo);
}

/*
**	Returns the object's vertex buffer object.
*/

GLuint		GameObject::GetVbo()
{
	return (_vbo);
}

/*
**	Returns the object's normal buffer object.
*/

GLuint		GameObject::GetNbo()
{
	return (_nbo);
}

/*
**	Returns the object's face UV buffer object.
**	Used to apply texture to faces.
*/

GLuint		GameObject::GetFubo()
{
	return (_fubo);
}

/*
**	Returns the object's texture buffer id.
*/

GLuint		GameObject::GetTextureID()
{
	return (_ObjTextureID);
}

/*
**	Returns the object's bmp texture object.
*/

t_bmp_texture		*GameObject::GetTexture()
{
	return (_objTexture);
}



/*
**	Set the object's bmp texture object. Use this to quickly switch between
**	loaded textures during runtime.
**	Make sure the new texture is the same size and is identically mapped
**	as the previous texture.
**	Otherwise the behavior is undefined.
**	Note that this does NOT free the old texture pointer.
**	Use LoadNewTexture(std::string path)
**	or ReplaceTexture(t_bmp_texture *newTexture) for this.
*/

int			GameObject::SwapTexture(t_bmp_texture *newTexture)
{
	if (HasTexture == true)
	{
		glDeleteTextures(1, &_ObjTextureID);
	}
	if (newTexture)
	{
		this->_objTexture = newTexture;
		glGenTextures(1, &_ObjTextureID);
	}
	else
	{
		HasTexture = false;
		return (-1);
	}
	return (0);
}

/*
**	Changes the t_bmp_texture by the new one, and delete and free the previous texture.
*/

int			GameObject::ReplaceTexture(t_bmp_texture *newTexture)
{
	if (HasTexture == true)
	{
		glDeleteTextures(1, &_ObjTextureID);
		free(_objTexture->data);
		free(_objTexture);
	}
	if (newTexture)
	{
		this->_objTexture = newTexture;
		glGenTextures(1, &_ObjTextureID);
	}
	else
	{
		HasTexture = false;
		this->_objTexture = NULL;
		return (-1);
	}
	return (0);
}

