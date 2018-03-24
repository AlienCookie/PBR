#ifndef EDIT_OBJ
#define EDIT_OBJ

#include "FileManager.h"
#include "Program.h"
#include "Shader.h"
#include "GLObject.h"
#include "GLBuffer.h"
#include "Texture.h"
#include "tiny_obj_loader.h"
#include <map>



class EditorObject : public GLObject
{
public:
	~EditorObject();

	class Builder {
		
	private:
			friend class EditorObject;
			// Elements we need to create an object
			GLuint objectVAO;
			GLuint objectProgram;
			GLuint buff;
			GLsizei count;
			
			GLuint albedoMap = -1;
			GLuint normalMap = -1;
			GLuint metallicMap = -1;
			GLuint roughnessMap = -1;
			GLuint aoMap = -1;
			GLuint cubeMap = -1;

			// 
			std::vector<vec3f> vPositions;
			std::vector<vec2f> vUVs;
			std::vector<vec3f> vNormals;

			std::vector<vec3f> vTangents;
			std::vector<vec3f> vBinormals;

			std::vector<unsigned> indices;
			//

			//
			tinyobj::material_t material;
			//

	public:
			Builder();
			Builder& addProgram(GLuint _program);
			Builder & addVAO(GLuint _VAO);

			// Buffer data
			Builder & addVPositions(std::vector<float> positions);
			Builder & addVPositions(std::vector<vec3f> positions);
			Builder & addVNormals(std::vector<float> normals);
			Builder & addVNormals(std::vector<vec3f> normals);
			Builder & addUVs(std::vector<float> uvs);
			Builder & addUVs(std::vector<vec2f> uvs);

			Builder & addTangents(std::vector<vec3f> _tangents);
			Builder & addBinormals(std::vector<vec3f> _binormals);

			Builder & addIndices(std::vector<unsigned> indices);

			// Materials
			Builder & addMaterial(tinyobj::material_t objectmaterial);


			EditorObject* build();
	};
	std::vector<vec3f> getVertPositions();
	std::vector<unsigned> getIndeces();

private:
	// to garantie object initialisation
	// only over builder
	EditorObject() = delete;

	EditorObject(EditorObject::Builder* builder);

	// store copy of vertices data in RAM (optional)
	// usefull for raycasting etc.
	std::vector<vec3f> vPositions;
	std::vector<unsigned> indices;


	tinyobj::material_t material;
};

#endif
