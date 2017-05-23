#ifndef ENGINEOBJECT_HPP
# define ENGINEOBJECT_HPP

#include "aleung-c_engine.hpp"

/*
**	The base class for the objects
**	Every Object, whatever his type, has a vao and a vbo.
**	Each has also the pointer to a possible texture.
**	This class allows similar code behaviors to be centralized,
**	such as texture swapping, accessors, etc ....
*/

class EngineObject
{
	public:
		std::string						Name;
		glm::vec3						Position;
		glm::vec3						Rotation;
		glm::vec3						Scale;
		float							ScaleValue;

		GLuint							GetVao();
		GLuint							GetVbo();

		std::string						TexturePath;

		GLuint							GetTextureID();
		t_bmp_texture					*GetTexture();

		// ----- Texture handling methods.
		bool							HasTexture();
		int								SwapTexture(t_bmp_texture *newTexture);
		int								ReplaceTexture(t_bmp_texture *newTexture);
		void							ClearTexture();

	protected:
		bool							_hasTexture;
		GLuint							_vao;
		GLuint							_vbo;

		GLuint							_objTextureID;
		t_bmp_texture					*_objTexture;
};


#endif
