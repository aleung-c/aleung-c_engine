#ifndef GAMETEXTOBJECT_HPP
# define GAMETEXTOBJECT_HPP

#include "aleung-c_engine.hpp"

class GameTextObject : public EngineObject
{
	public:
		GameTextObject(std::string name, std::string text);
		~GameTextObject();

		std::string						Text;
		glm::vec3						Color;

		void							InitValues();
		void							SetBuffers();
};

#endif
