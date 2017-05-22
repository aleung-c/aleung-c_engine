#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Textures															//
//	Handle the per object texturing										//
//																		//
// --------------------------------------------------------------------	//
/*
**	We will say that each object only has one texture...
*/

void	GameEngineController::loadObjectTexture(GameObject *Object)
{
	GLuint			uniform_mat;

	// we will only activate the number 0 texture. it may go up to 32 on newest graphic cards.
	// mobile devices usually have only 2.
	glActiveTexture(GL_TEXTURE0);

	// "Bind" the object's texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, Object->GetTextureID());

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Object->GetTexture()->width, Object->GetTexture()->height, 0,
		GL_BGR, GL_UNSIGNED_BYTE, Object->GetTexture()->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// bind texture to fragment shader uniform sampler2D
	uniform_mat = glGetUniformLocation(MainShaderProgramme, "texture_0");
	glUniform1i(uniform_mat, 0);

	// set fshader bool to true -> there is a texture loaded.
	glUniform1i(glGetUniformLocation(MainShaderProgramme, "has_texture"), GL_TRUE);
}
