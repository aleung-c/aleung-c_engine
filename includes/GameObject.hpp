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
		// -----------------------	Construct init Methods
		void							InitValues();
		void							GetObjValues(FILE *file);
		void							CreateObjFaces();
		void							SetBuffers();
		void							LoadTexture();
		GLuint							GetTextureID();
		t_bmp_texture					&GetTexture();

		// -----------------------	Drawing Methods
		void							DrawObject();
	
	private:
		// OpenGl - Obj loading
		GLuint							_vao;

		GLuint							_fvbo;
		GLuint							_vbo;
		GLuint							_nbo;
		//GLuint							_cbo; // color buffer. unused.
		GLuint							_fubo;

		std::vector<glm::vec4>			_objFacesVertices;
		std::vector<glm::vec2>			_objFacesUVs;

		std::vector<glm::vec4>			_objVertices;
		std::vector<glm::vec3>			_objNormals;
		std::vector<glm::vec2>			_objUVs;

		std::vector<int>				_objVertexIndices;
		std::vector<int>				_objUVIndices;
		std::vector<int>				_objNormalIndices;

		// Texturing;
		t_bmp_texture					_objTexture;
		GLuint							_ObjTextureID;

};

#endif
