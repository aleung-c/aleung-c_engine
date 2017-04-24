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
	ObjPath = "";
	Name = objName;
	InitValues();

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
	// set initial values to zero.
	InitValues();
	// Parse the obj file to scoop values.
	GetObjValues(file);
	// set bounding box center -> object math center.
	BoundingBoxCenter.x = (BoundingBoxMin.x + BoundingBoxMax.x) / 2;
	BoundingBoxCenter.y = (BoundingBoxMin.y + BoundingBoxMax.y) / 2;
	BoundingBoxCenter.z = (BoundingBoxMin.z + BoundingBoxMax.z) / 2;

	// ----- PRINT Bounding box
	// std::cout << "min = " << BoundingBoxMin.x << "x "
	// 			<< BoundingBoxMin.y << "y "
	// 			<< BoundingBoxMin.z << "z" << std::endl;

	// std::cout << "center = " << BoundingBoxCenter.x << "x "
	// 			<< BoundingBoxCenter.y << "y "
	// 			<< BoundingBoxCenter.z << "z" << std::endl;

	// std::cout << "max = " << BoundingBoxMax.x << "x "
	// 			<< BoundingBoxMax.y << "y "
	// 			<< BoundingBoxMax.z << "z" << std::endl;

	// create faces from indexes.
	CreateObjFaces();
	// set opengl buffers
	SetBuffers();
	// load texture.
	LoadTexture();
	// Add this object to the engine's list of objects.
	GameEngineController::Instance().GameObjectList.push_back(this);
}

void		GameObject::InitValues()
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

void	GameObject::CreateObjFaces()
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
}

void		GameObject::SetBuffers()
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
	
	// the color buffer cbo.
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

void		GameObject::LoadTexture()
{
	std::string texPath = ObjPath;
	texPath.resize(ObjPath.size() - 4);
	texPath.append(".bmp", 4);
	if ((GameEngineController::LoadTextureFile(&_objTexture, texPath)) == -1)
	{
		HasTexture = false;
		return ;
	}
	HasTexture = true;
	glGenTextures(1, &_ObjTextureID);
}

GLuint		GameObject::GetTextureID()
{
	return (_ObjTextureID);
}

t_bmp_texture		&GameObject::GetTexture()
{
	return (_objTexture);
}

void		GameObject::DrawObject()
{
	if (HasModel == true)
	{
		// ------ load the uvs for the object - LOCATION = 1
		glBindBuffer(GL_ARRAY_BUFFER, _fubo);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		// ------ load vertex and draw them - LOCATION = 0
		// ------ To display only points from vertex
		// glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		// glEnableVertexAttribArray(0);
		// glDrawArrays (GL_POINTS, 0, _objVertices.size());
		// ------ To display triangles from faces vertex
		glBindBuffer(GL_ARRAY_BUFFER, _fvbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, _objFacesVertices.size());

		// ----- disable all after draw;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}
