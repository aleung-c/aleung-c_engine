#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	The class for a game object that is textual.						//
//	A new class seems useful because the text are not handled the same	//
//	way as a 3d model.													//
// --------------------------------------------------------------------	//

GameUIObject::GameUIObject(std::string name, std::string imgPath)
{
	Name = name;
	TexturePath = imgPath;
	InitValues();
	SetBuffers();
	LoadTexture();

	// Add this object to the engine's list of objects.
	GameEngineController::Instance().GameUIObjectList.push_back(this);
}

GameUIObject::GameUIObject(std::string name)
{
	Name = name;
	InitValues();
	HasTexture = false;
	// Add this object to the engine's list of objects.
	GameEngineController::Instance().GameUIObjectList.push_back(this);
}

void		GameUIObject::InitValues()
{
	Position = glm::vec3(0.0, 0.0, 0.0);
	Scale = 1.0;
}

void		GameUIObject::SetBuffers()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
}

void		GameUIObject::LoadTexture()
{
	if ((GameEngineController::LoadTextureFile(&_objTexture, TexturePath)) == -1)
	{
		HasTexture = false;
		return ;
	}
	HasTexture = true;
	glGenTextures(1, &_objTextureID);

	// // Generate texture
	// 	GLuint	texture;
	// 	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, _objTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		_objTexture.width,
		_objTexture.height,
		0, GL_BGR, GL_UNSIGNED_BYTE, _objTexture.data);

	// // Set texture options
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

GLuint	GameUIObject::GetVao()
{
	return (_vao);
}

GLuint	GameUIObject::GetVbo()
{
	return (_vbo);
}

t_bmp_texture		GameUIObject::GetTexture()
{
	return (_objTexture);
}

GLuint				GameUIObject::GetTextureID()
{
	return (_objTextureID);
}
