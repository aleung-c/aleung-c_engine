#include "../includes/aleung-c_engine.hpp"

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
	MainCamera->Position = glm::vec3(0.0, 5.0, 10.0);
	MatView = glm::lookAt(
		MainCamera->Position,
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
**	Rotation is not working as intended. The pivot point is incorrect.
**	Lots of inversions of matrices were tested, without success.
**	Maybe the problem does not come from the matrices.
*/

void		GameEngineController::applyMatricesToObject(GameObject *Object)
{
	// Model matrice, multiplied on the spot with glm. Note that the order is reversed,
	// As it seems glm reverses the way it calculates. (right hand multiplication)
	MatModel = glm::mat4();

	// translation
	MatModel = glm::translate(MatModel, Object->Position);

	// scaling
	MatModel = glm::scale(MatModel, Object->Scale);

	// added offset for recentering.
	MatModel = glm::translate(MatModel, Object->BoundingBoxCenter);
	// rotation
	MatModel = glm::rotate(MatModel, glm::radians(Object->Rotation.x), glm::vec3(1.0, 0.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Rotation.y), glm::vec3(0.0, 1.0, 0.0));
	MatModel = glm::rotate(MatModel, glm::radians(Object->Rotation.z), glm::vec3(0.0, 0.0, 1.0));
	// remove offset for recentering.
	MatModel = glm::translate(MatModel, -Object->BoundingBoxCenter);

	// // Separated matrice version, same end;
		// MatModelTranslation = glm::translate(glm::mat4(), Object->Position);
		// MatModelRecenter = glm::translate(glm::mat4(), Object->BoundingBoxCenter);
		// MatModelRecenterInverted = glm::translate(glm::mat4(), -Object->BoundingBoxCenter);
		// MatModelRotation = glm::rotate(glm::mat4(), glm::radians(Object->Rotation.x), glm::vec3(1.0, 0.0, 0.0));
		// MatModelRotation = glm::rotate(MatModelRotation, glm::radians(Object->Rotation.y), glm::vec3(0.0, 1.0, 0.0));
		// MatModelRotation = glm::rotate(MatModelRotation, glm::radians(Object->Rotation.z), glm::vec3(0.0, 0.0, 1.0));
		// MatModelScaling = glm::scale(glm::mat4(), Object->Scale);
		// MatModelIdentity = glm::mat4(1.0);

		// MatModel = MatModelTranslation * MatModelScaling * MatModelRecenter * MatModelRotation * MatModelRecenterInverted * MatModelIdentity;

	// Final MVP matrice merging.
	MatMVP = MatPerspectiveProjection * MatView * MatModel;	

	// Send it to shader.
	GLint uniform_mat = glGetUniformLocation(MainShaderProgramme, "mvp_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatMVP[0][0]);
}
