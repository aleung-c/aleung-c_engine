#ifndef GAMEUIOBJECT_HPP
# define GAMEUIOBJECT_HPP

#include "aleung-c_engine.hpp"

class GameUIObject
{
	public:
		GameUIObject(std::string name, std::string imgPath);
		GameUIObject(std::string name);
		~GameUIObject();

		bool							HasTexture;
		
		std::string						Name;
		std::string						TexturePath;


		glm::vec3						Position;
		GLfloat							Scale;

		void							InitValues();
		void							SetBuffers();
		void							LoadTexture();
		GLuint							GetVao();
		GLuint							GetVbo();
		t_bmp_texture					GetTexture();
		GLuint							GetTextureID();

	private:
		GLuint							_vao;
		GLuint							_vbo;

		GLuint							_objTextureID;
		t_bmp_texture					_objTexture;

};

#endif