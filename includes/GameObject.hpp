#ifndef GAMEOBJECT_HPP
# define GAMEOBJECT_HPP

#include "aleung-c_engine.hpp"

class GameObject
{
	public:
		// GameObject constructors
		GameObject(std::string objName);
		GameObject(std::string objName, std::string path);

		~GameObject();

		// GameObject public variables.
		std::string						Name;
		std::string						ObjPath;
		bool							HasModel;
		bool							HasTexture;

		glm::vec3						Position;
		glm::vec3						Rotation;
		glm::vec3						Scale;

		glm::vec3						BoundingBoxMin;
		glm::vec3						BoundingBoxMax;
		glm::vec3						BoundingBoxCenter;

		// GameObject's public methods.

		// ----- User Called methods
		void							LoadNewTexture(std::string path);


		// Accessors
		GLuint							GetVao();
		int								GetNbFaceVertices();
		int								GetNbFaces();
		GLuint							GetFvbo();
		GLuint							GetVbo();
		GLuint							GetNbo();
		GLuint							GetFubo();
		GLuint							GetTextureID();
		t_bmp_texture					*GetTexture();
		int								SwapTexture(t_bmp_texture *newTexture);
		int								ReplaceTexture(t_bmp_texture *newTexture);
	
	private:
		// ----- Construct init Methods
		void							initValues();
		void							getObjValues(FILE *file);
		void							createObjFaces();
		void							setBuffers();
		void							loadTexture();

		// OpenGl - Obj loading
		GLuint							_vao;

		GLuint							_fvbo; // face vertex buffer object
		GLuint							_vbo; // vertex buffer object. not used for drawing.
		GLuint							_nbo; // normal buffer object - unused. Usually used for lighting.
		//GLuint							_cbo; // color buffer - unused.
		GLuint							_fubo; // face UV buffer object.

		int								_nbFaceVertices;
		int								_nbFaces;
		std::vector<glm::vec4>			_objFacesVertices;
		std::vector<glm::vec2>			_objFacesUVs;

		std::vector<glm::vec4>			_objVertices;
		std::vector<glm::vec3>			_objNormals;
		std::vector<glm::vec2>			_objUVs;

		std::vector<int>				_objVertexIndices;
		std::vector<int>				_objUVIndices;
		std::vector<int>				_objNormalIndices;

		// Texturing;
		t_bmp_texture					*_objTexture;
		GLuint							_ObjTextureID;

};

#endif
