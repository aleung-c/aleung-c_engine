#ifndef GAMEOBJECT_HPP
# define GAMEOBJECT_HPP

#include "aleung-c_engine.hpp"

class GameObject : public EngineObject
{
	public:
		// GameObject constructors
		GameObject(std::string objName);
		GameObject(std::string objName, std::string path);

		~GameObject();

		// GameObject public variables.
		std::string						ObjPath;
		bool							HasModel;

		glm::vec3						BoundingBoxMin;
		glm::vec3						BoundingBoxMax;
		glm::vec3						BoundingBoxCenter;

		// GameObject's public methods.

		// Accessors
		int								GetNbFaceVertices();
		int								GetNbFaces();
		GLuint							GetFvbo();
		GLuint							GetNbo();
		GLuint							GetFubo();

	private:
		// ----- Construct init Methods
		void							initValues();
		void							getObjValues(FILE *file);
		void							createObjFaces();
		void							setBuffers();


		// OpenGl - Obj loading
		GLuint							_fvbo; // face vertex buffer object -> for 3d faces
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
};

#endif
