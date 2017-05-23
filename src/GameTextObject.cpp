#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	The class for a game object that is textual.						//
//	A new class seems useful because the text are not handled the same	//
//	way as a 3d model.													//
// --------------------------------------------------------------------	//

GameTextObject::GameTextObject(std::string name, std::string text)
{
	Name = name;
	Text = text;
	InitValues();
	SetBuffers();
	_hasTexture = false;
	// Add this object to the engine's list of objects.
	GameEngineController::Instance().GameTextObjectList.push_back(this);
}

void		GameTextObject::InitValues()
{
	Position = glm::vec3(0.0, 0.0, 0.0);
	Color = glm::vec3(1.0, 1.0, 1.0);
	ScaleValue = 1.0;
}

void		GameTextObject::SetBuffers()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
}
