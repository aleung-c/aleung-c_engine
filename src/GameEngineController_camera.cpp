#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Camera																//
//	The camera is a separate matrix that will be applied to				//
//	every other object in our scene.									//
//	It is made on a "position" and a "look at" basis.					//
//																		//
// --------------------------------------------------------------------	//

/*
**	This method allows the game developper to "extract" the camera with a
**	pointer.
*/

GameObject		*GameEngineController::GetCamera()
{
	return (MainCamera);
}

/*
**	This is the same with the LookAt glm::vec3 variable.
*/

glm::vec3		*GameEngineController::GetCameraLookAt()
{
	return (&CameraLookAt);
}

void			GameEngineController::SetCamera(GameObject *NewCamera)
{
	if (MainCamera)
	{
		delete MainCamera;
	}
	MainCamera = NewCamera;
}

void			GameEngineController::SetCameraLookAt(glm::vec3 new_look_pos)
{
	CameraLookAt = new_look_pos;
}
