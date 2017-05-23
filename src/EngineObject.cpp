#include "../includes/aleung-c_engine.hpp"

/*
**	Returns the object's vertex array object.
*/

GLuint		EngineObject::GetVao()
{
	return (_vao);
}

/*
**	Returns the object's vertex buffer object.
*/

GLuint		EngineObject::GetVbo()
{
	return (_vbo);
}

/*
**	Returns the object's texture buffer id.
*/

GLuint		EngineObject::GetTextureID()
{
	return (_objTextureID);
}

/*
**	Returns the object's bmp texture object.
*/

t_bmp_texture		*EngineObject::GetTexture()
{
	return (_objTexture);
}

bool				EngineObject::HasTexture()
{
	return (_hasTexture);
}

/*
**	Set the object's bmp texture object. Use this to quickly switch between
**	loaded textures during runtime.
**	Make sure the new texture is the same size and is identically mapped
**	as the previous texture.
**	Otherwise the behavior is undefined.
**
**	Note that this does NOT free the old texture pointer.
**	In some cases, you will have leaks using this.
**	You may want to use ClearTexture() before using this,
**	or only use this on untextured objects.
**	you should also look at LoadNewTexture(std::string path)
**	or ReplaceTexture(t_bmp_texture *newTexture).
*/

int			EngineObject::SwapTexture(t_bmp_texture *newTexture)
{
	if (_hasTexture == true)
	{
		glDeleteTextures(1, &_objTextureID);
	}
	if (newTexture)
	{
		std::cout << "swap texture!" << std::endl;
		this->_objTexture = newTexture;
		glGenTextures(1, &_objTextureID);
		_hasTexture = true;
	}
	else
	{
		_hasTexture = false;
		return (-1);
	}
	return (0);
}

/*
**	Changes the t_bmp_texture by the new one, and delete and free the previous texture.
*/

int			EngineObject::ReplaceTexture(t_bmp_texture *newTexture)
{
	if (_hasTexture == true)
	{
		glDeleteTextures(1, &_objTextureID);
		free(_objTexture->data);
		free(_objTexture);
	}
	if (newTexture)
	{
		this->_objTexture = newTexture;
		glGenTextures(1, &_objTextureID);
		_hasTexture = true;
	}
	else
	{
		_hasTexture = false;
		this->_objTexture = NULL;
		return (-1);
	}
	return (0);
}

/*
**	Delete the current texture on the object, freeing the pointer as it should be.
*/

void		EngineObject::ClearTexture()
{
	if (_hasTexture == true)
	{
		glDeleteTextures(1, &_objTextureID);
		free(_objTexture->data);
		free(_objTexture);
		_hasTexture = false;
	}
}
