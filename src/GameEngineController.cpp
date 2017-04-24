#include "../includes/aleung-c_engine.hpp"

GameEngineController GameEngineController::m_instance = GameEngineController();

// default constructor
GameEngineController::GameEngineController()
:
DebugMode(false),
EngineInitialized(false)

{

}

// default destructor
GameEngineController::~GameEngineController()
{

}

GameEngineController& GameEngineController::Instance()
{
	return m_instance;
}

void		GameEngineController::InitEngine(int windowWidth, int windowHeight, std::string windowName)

{
	WindowWidth = windowWidth;
	WindowHeight = windowHeight;
	WindowName = windowName;
	if (InitGLFW() == -1
		|| InitOpenGL() == -1
		|| InitFreeType() == -1)
	{
		std::cout << "Initialization error. Exiting..." << std::endl;
		exit (-1);
	}
	LoadShaders();
	LoadMatrices();
	EngineInitialized = true;
	if (DebugMode)
	{
		CheckForOpenGLErrors();
		std::cout << "GameEngineController: Engine initialized!" << std::endl;
	}
}

void	GameEngineController::CheckForOpenGLErrors()
{
	int error = 0;

	// check if main 3d perspective shader is compiled and linked;
	GLint isLinked = 0;
	glGetProgramiv(MainShaderProgramme, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		printf(KRED "Error: Main Shader programme NOT linked%s\n", KRESET);
		error = -1;
	}
	else
		printf(KGRN "Main Shader programme linked%s\n", KRESET);
	// check if ortho shader is compiled and linked;
	isLinked = 0;
	glGetProgramiv(OrthoShaderProgramme, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		printf(KRED "Error: Text Shader programme NOT linked%s\n", KRESET);
		error = -1;
	}
	else
		printf(KGRN "Text Shader programme linked%s\n", KRESET);
	if (error == 0)
		return ;

	// check for opengl errors.
	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf(KRED "OpenGL Error: %u%s\n", err, KRESET);
	exit (-1);
}

// --------------------------------------------------------------------	//
//																		//
//	Library inits														//
//																		//
// --------------------------------------------------------------------	//

int		GameEngineController::InitGLFW()
{
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit ())
	{
		std::cout << "ERROR: could not start GLFW3" << std::endl;
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName.c_str(), NULL, NULL);
	if (!Window)
	{
		std::cout << "ERROR: could not open window with GLFW3" << std::endl;
		glfwTerminate();
		return (-1);
	}
	// Create context
	glfwMakeContextCurrent (Window);
	return (0);
}


int		GameEngineController::InitOpenGL()
{
	// get version info
	const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString (GL_VERSION); // version as a string
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif
	return (0);
}

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

void	GameEngineController::LoadMatrices()
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
	CameraFov = 90.0;
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

void GameEngineController::ApplyMatricesToObject(GameObject *Object)
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

	// Final MVP matrice mergin.
	MatMVP = MatPerspectiveProjection * MatView * MatModel;

	// Send it to shader.
	GLint uniform_mat = glGetUniformLocation(MainShaderProgramme, "mvp_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatMVP[0][0]);
}

// --------------------------------------------------------------------	//
//																		//
//	Textures															//
//	Handle the per object texturing										//
//																		//
// --------------------------------------------------------------------	//
/*
**	We will say that each object only has one texture...
*/

void	GameEngineController::LoadObjectTexture(GameObject *Object)
{
	GLuint			uniform_mat;

	// we will only activate the number 0 texture. it may go up to 32 on newest graphic cards.
	// mobile devices usually have only 2.
	glActiveTexture(GL_TEXTURE0);

	// "Bind" the object's texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, Object->GetTextureID());

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Object->GetTexture().width, Object->GetTexture().height, 0,
		GL_BGR, GL_UNSIGNED_BYTE, Object->GetTexture().data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// bind texture to fragment shader uniform sampler2D
	uniform_mat = glGetUniformLocation(MainShaderProgramme, "texture_0");
	glUniform1i(uniform_mat, 0);

	// set fshader bool to true -> there is a texture loaded.
	glUniform1i(glGetUniformLocation(MainShaderProgramme, "has_texture"), GL_TRUE);
}

// --------------------------------------------------------------------	//
//																		//
//	Engine side drawing													//
//																		//
// --------------------------------------------------------------------	//

/*
**	Main drawing function. This will be called at each loop turn.
*/

void	GameEngineController::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw3DModels();
	DrawUIObjects();
	DrawTextObjects();

	// display on screen.
	glfwSwapBuffers(Window);
}

/*
**	Run through and display each GameObject that has a model/texture.
*/

void	GameEngineController::Draw3DModels()
{
	glUseProgram(MainShaderProgramme);

	// Reset Camera VIEW matrix for camera movement.
	MatView = glm::lookAt(MainCamera->Position, CameraLookAt, glm::vec3(0.0, 2.0, 0.0));

	// draw 3d objects
	// run through each object to set their matrices and textures and draw them on screen.
	for (std::vector<GameObject *>::iterator it = GameObjectList.begin();
		it != GameObjectList.end();
		it++)
	{
		// texture loading.
		if ((*it)->HasTexture == true)
			LoadObjectTexture(*it);
		else
			glUniform1i(glGetUniformLocation(MainShaderProgramme, "has_texture"), GL_FALSE);
		// opengl buffer loading.
		if ((*it)->HasModel == true)
		{
			ApplyMatricesToObject(*it);
			(*it)->DrawObject();
		}
	}
}

/*
**	Run through and display each GameTextObject and render the text.
**	See GameEngineController_freetype_font.cpp
*/

void	GameEngineController::DrawUIObjects()
{
	glUseProgram(OrthoShaderProgramme);
	// set projection matrice.
	GLint uniform_mat = glGetUniformLocation(OrthoShaderProgramme, "projection_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatOrthographicProjection[0][0]);
	glUniform1i(glGetUniformLocation(OrthoShaderProgramme, "IsText"), GL_FALSE);
	for (std::vector<GameUIObject *>::iterator it = GameUIObjectList.begin();
		it != GameUIObjectList.end();
		it++)
	{
		RenderUIObject(*it);
	}
}

/*
**	Run through and display each GameUIObject and render them.
**	See GameEngineController_UI.cpp for rendering. Uses mostly the same
**	rendering method as the textObject, but has only one quad to draw.
*/

void	GameEngineController::DrawTextObjects()
{
	glUseProgram(OrthoShaderProgramme);
	// set projection matrice.
	GLint uniform_mat = glGetUniformLocation(OrthoShaderProgramme, "projection_matrix");
	if (uniform_mat != -1)
		glUniformMatrix4fv(uniform_mat, 1, GL_FALSE, &MatOrthographicProjection[0][0]);
	glUniform1i(glGetUniformLocation(OrthoShaderProgramme, "IsText"), GL_TRUE);
	for (std::vector<GameTextObject *>::iterator it = GameTextObjectList.begin();
		it != GameTextObjectList.end();
		it++)
	{
		RenderText(*it);
	}
}

// User access methods

GameObject							*GameEngineController::GetCamera()
{
	return (MainCamera);
}

glm::vec3							*GameEngineController::GetCameraLookAt()
{
	return (&CameraLookAt);
}

void								GameEngineController::SetCamera(GameObject *NewCamera)
{
	if (MainCamera)
	{
		delete MainCamera;
	}
	MainCamera = NewCamera;
}

void								GameEngineController::SetCameraLookAt(glm::vec3 new_look_pos)
{
	CameraLookAt = new_look_pos;
}
