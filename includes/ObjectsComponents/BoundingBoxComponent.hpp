#ifndef BOUNDINGBOXCOMPONENT_HPP
# define BOUNDINGBOXCOMPONENT_HPP

#include "../aleung-c_engine.hpp"

class BoundingBoxComponent
{
	public:
		glm::vec3						Min;
		glm::vec3						Max;
		glm::vec3						Center;

		float							Width;
		float							Height;
		float							Depth;
};

#endif
