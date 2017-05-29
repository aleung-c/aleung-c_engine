#ifndef TRANSFORMCOMPONENT_HPP
# define TRANSFORMCOMPONENT_HPP

#include "../aleung-c_engine.hpp"

class TransformComponent
{
	public:
		glm::vec3						Position;
		glm::vec3						Rotation;
		glm::vec3						Scale;
		float							ScaleValue;
};

#endif
