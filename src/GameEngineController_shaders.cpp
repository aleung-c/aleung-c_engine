#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Shaders inits														//
//																		//
// --------------------------------------------------------------------	//

void	GameEngineController::LoadShaders()
{
	// -------------------------------------------------------------------------- //
	//	3d model Shaders -> main shader											  //
	// -------------------------------------------------------------------------- //
	// Go get Position shader
	VertexShader_1 = GetFileContent("./aleung-c_engine/shaders/vshader_1.vs");
	FragmentShader_1 = GetFileContent("./aleung-c_engine/shaders/fshader_1.fs");

	// Create shader programme
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const char * const *)&VertexShader_1, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const char * const *)&FragmentShader_1, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram ();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram (shader_programme);
	MainShaderProgramme = shader_programme;

	// -------------------------------------------------------------------------- //
	//	Orthographic Shaders -> UI and text										  //
	// -------------------------------------------------------------------------- //
	TextVShader = GetFileContent("./aleung-c_engine/shaders/ortho_vshader_1.vs");
	TextFShader = GetFileContent("./aleung-c_engine/shaders/ortho_fshader_1.fs");

	// Create shader programme
	GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs2, 1, (const char * const *)&TextVShader, NULL);
	glCompileShader(vs2);
	GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs2, 1, (const char * const *)&TextFShader, NULL);
	glCompileShader(fs2);

	GLuint shader_programme2 = glCreateProgram ();
	glAttachShader(shader_programme2, fs2);
	glAttachShader(shader_programme2, vs2);
	glLinkProgram (shader_programme2);
	OrthoShaderProgramme = shader_programme2;
}
