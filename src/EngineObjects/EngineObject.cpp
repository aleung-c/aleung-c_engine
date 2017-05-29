#include "../../includes/aleung-c_engine.hpp"

EngineObject::EngineObject()
:
	Visible(true)
{

}

EngineObject::~EngineObject()
{

}

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
