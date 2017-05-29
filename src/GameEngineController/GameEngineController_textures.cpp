#include "../../includes/aleung-c_engine.hpp"

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
	glBindTexture(GL_TEXTURE_2D, Object->Texture.GetTextureID());

	glTexStorage2D(GL_TEXTURE_2D, Settings.TextureMipMapValue, GL_RGBA,
		Object->Texture.GetTexture()->width, Object->Texture.GetTexture()->height);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		Object->Texture.GetTexture()->width,
		Object->Texture.GetTexture()->height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, Object->Texture.GetTexture()->data);

	glGenerateMipmap(GL_TEXTURE_2D);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// bind texture to fragment shader uniform sampler2D
	uniform_mat = glGetUniformLocation(MainShaderProgramme, "texture_0");
	glUniform1i(uniform_mat, 0);

	// set fshader bool to true -> there is a texture loaded.
	glUniform1i(glGetUniformLocation(MainShaderProgramme, "has_texture"), GL_TRUE);
}
