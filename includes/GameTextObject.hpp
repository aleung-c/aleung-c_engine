#ifndef GAMETEXTOBJECT_HPP
# define GAMETEXTOBJECT_HPP

#include "aleung-c_engine.hpp"

class GameTextObject
{
	public:
		GameTextObject(std::string name, std::string text);
		~GameTextObject();

		std::string						Name;
		std::string						Text;
		glm::vec3						Color;

		glm::vec3						Position;
		GLfloat							Scale;

		void							InitValues();
		void							SetBuffers();
		GLuint							GetVao();
		GLuint							GetVbo();

	private:
		GLuint							_vao;
		GLuint							_vbo;

};

#endif
