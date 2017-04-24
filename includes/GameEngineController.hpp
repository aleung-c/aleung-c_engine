#ifndef GAMEENGINECONTROLLER_HPP
# define GAMEENGINECONTROLLER_HPP

# include "aleung-c_engine.hpp"

/*
**	This engine is a GameObject collector. When a new GameObject
**	is created, it goes into the engine's list of object,
**	and this engine will draw it automatically, applying matrices
**	and stuff.
*/

class	GameEngineController
{
	public:
		// this is a singleton
		static GameEngineController&		Instance();

		int									WindowWidth;
		int									WindowHeight;
		std::string							WindowName;
		GLFWwindow							*Window;

		bool								DebugMode;
		bool								EngineInitialized;

		// ------ Engine's objects handling ------
		std::vector<GameObject *>			GameObjectList;
		std::vector<GameTextObject *>		GameTextObjectList;
		std::vector<GameUIObject *>			GameUIObjectList;


		// ------ Engine's Camera handling ------
		GameObject							*MainCamera;
		glm::vec3							CameraLookAt;

		// ------ Shaders handling ------
		char								*VertexShader_1;
		char								*FragmentShader_1;

		char								*TextVShader;
		char								*TextFShader;

		GLuint								MainShaderProgramme;
		GLuint								OrthoShaderProgramme;

		// ------ Matrix handling ------
		// Model
		glm::mat4							MatModelIdentity;
		glm::mat4							MatModelTranslation;
		glm::mat4							MatModelRecenter;
		glm::mat4							MatModelRotation;
		glm::mat4							MatModelScaling;

		// View -> camera
		float								CameraNear;
		float								CameraFar;
		float								CameraFov;
		float								CameraAspect;

		// Projection -> persp or ortho
		glm::mat4							MatPerspectiveProjection;
		glm::mat4							MatOrthographicProjection;

		// Matrices merge
		glm::mat4							MatModel;
		glm::mat4							MatView;

		glm::mat4							MatMVP;

		// ------ Freetype lib fonts ------
		FT_Library							FT_Lib;
		FT_Face								Face;

		// map of asciis 128 characters.
		std::map<GLchar, Character>			Characters;


		GameEngineController();
		~GameEngineController();

		void								InitEngine(int windowWidth, int windowHeight, std::string WindowName);
		
		int									InitGLFW();
		int									InitOpenGL();
		int									InitFreeType();
		void								LoadFreeTypesCharacters();


		void								CheckForOpenGLErrors();

		void								LoadShaders();

		static char							*GetFileContent(std::string file_path);
		static int							LoadTextureFile(t_bmp_texture *texture, std::string path);

		void								LoadMatrices();
		void								ApplyMatricesToObject(GameObject *Object);
		void								LoadObjectTexture(GameObject *Object);
		void								RenderText(GameTextObject *obj);

		void								Draw();
		void								Draw3DModels();
		void								DrawUIObjects();
		void								DrawTextObjects();

		void								RenderUIObject(GameUIObject *obj);

		// User access
		GameObject							*GetCamera();
		glm::vec3							*GetCameraLookAt();
		void								SetCamera(GameObject *NewCamera);
		void								SetCameraLookAt(glm::vec3 new_look_pos);

	private:
		static GameEngineController m_instance;

		// ------ Font tmp vars ------
		int									tmp_x;
		int									tmp_y;
		Character							ch;
		GLfloat								xpos;
		GLfloat								ypos;
		GLfloat								w;
		GLfloat								h;
};

#endif
