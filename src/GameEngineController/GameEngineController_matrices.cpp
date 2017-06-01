#include "../../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Matrices															//
//	This creates the usual MODEL - VIEW - PROJECTION					//
//	matrice.															//
//																		//
// --------------------------------------------------------------------	//

/*
**	This method initialize the VIEW and PROJECTION matrices.
**	The MODEL matrice will be set for each object at runtime.
**	Same goes for the merge into the MVP matrice.
*/

void		GameEngineController::loadMatrices()
{
	// View matrices init;
	MainCamera = new GameObject("MainCamera");
	MainCamera->Transform.Position = glm::vec3(0.0, 5.0, 10.0);
	MatView = glm::lookAt(
		MainCamera->Transform.Position,
		CameraLookAt, // regarde l'origine
		glm::vec3(0.0, 2.0, 0.0)  // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers) 
	);

	// Project matrices init;
	CameraNear = 0.1;
	CameraFar = 100.0;
	CameraFov = 45.0;
	CameraAspect = 1.77; // 4/3, 16/9, etc 1 = 4/4
	MatPerspectiveProjection = glm::perspective(CameraFov, CameraAspect, CameraNear, CameraFar);

	MatOrthographicProjection = glm::ortho(0.0f, (float)WindowWidth, 0.0f, (float)WindowHeight);
}

/*
**	For one object of our scene, apply model view projection matrices.
**	The rotation pivot point is correct, just make sure your
**	obj model is exported from its local center.
*/

void		GameEngineController::applyMatricesToObject(GameObject *Object)
{
	// Model matrice, multiplied on the spot with glm. Note that the order is reversed,
	// As it seems glm reverses the way it calculates. (right hand multiplication)
	MatModel = glm::mat4();

	// translation
	MatModel = glm::translate(MatModel, Object->Transform.Position);

	// scaling
	MatModel = glm::scale(MatModel, Object->Transform.Scale);


	// added offset for recentering.
	MatModel = glm::translate(MatModel, Object->BoundingBox.Center);
	// rotation
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Transform.Rotation.z), glm::vec3(0.0, 0.0, 1.0));
	// remove offset for recentering.
	MatModel = glm::translate(MatModel, -Object->BoundingBox.Center);


	// update bounding box with model matrice (vec3/4 conversion)
	// _tmpVec = glm::vec4(Object->BoundingBox.Min.x, Object->BoundingBox.Min.y, Object->BoundingBox.Min.z, 1.0);
	// _tmpVec = _tmpVec * MatModel;
	// Object->BoundingBox.Min = glm::vec3(_tmpVec.x, _tmpVec.y, _tmpVec.z);

	// _tmpVec = glm::vec4(Object->BoundingBox.Max.x, Object->BoundingBox.Max.y, Object->BoundingBox.Max.z, 1.0);
	// _tmpVec = _tmpVec * MatModel;
	// Object->BoundingBox.Max = glm::vec3(_tmpVec.x, _tmpVec.y, _tmpVec.z);

	// _tmpVec = glm::vec4(Object->BoundingBox.Center.x, Object->BoundingBox.Center.y, Object->BoundingBox.Center.z, 1.0);
	// _tmpVec = _tmpVec * MatModel;
	// Object->BoundingBox.Center = glm::vec3(_tmpVec.x, _tmpVec.y, _tmpVec.z);

	// Final MVP matrice merging.
	MatMVP = MatPerspectiveProjection * MatView * MatModel;	

	// Send it to shader.
	GLint uniform_mat = glGetUniformLocation(MainShaderProgramme, "mvp_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatMVP[0][0]);

	// update bounding box
	glm::rotate(Object->BoundingBox.Min, glm::radians(Object->Transform.Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	glm::rotate(Object->BoundingBox.Min, glm::radians(Object->Transform.Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	glm::rotate(Object->BoundingBox.Min, glm::radians(Object->Transform.Rotation.z), glm::vec3(0.0, 0.0, 1.0));

	glm::rotate(Object->BoundingBox.Center, glm::radians(Object->Transform.Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	glm::rotate(Object->BoundingBox.Center, glm::radians(Object->Transform.Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	glm::rotate(Object->BoundingBox.Center, glm::radians(Object->Transform.Rotation.z), glm::vec3(0.0, 0.0, 1.0));

	glm::rotate(Object->BoundingBox.Max, glm::radians(Object->Transform.Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	glm::rotate(Object->BoundingBox.Max, glm::radians(Object->Transform.Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	glm::rotate(Object->BoundingBox.Max, glm::radians(Object->Transform.Rotation.z), glm::vec3(0.0, 0.0, 1.0));
}
