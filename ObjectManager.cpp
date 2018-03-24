#include "stdafx.h"
#include "ObjectManager.h"
#include "GLBuffer.h"

#include <vector>
#include <string>


#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

_inline vec3f mult(vec3f v, float c) {
	return{ v.x * c, v.y * c, v.z * c };
}

inline vec3f sub(vec3f v1, vec3f v2)
{
	return{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline vec3f add(vec3f v1, vec3f v2)
{
	return{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline vec2f sub(vec2f v1, vec2f v2)
{
	return{ v1.x - v2.x, v1.y - v2.y };
}

bool vertexExist(std::vector<vec3f> &pos, std::vector<vec3f> &norm, std::vector<vec2f> &texCr, vec3f &posV, vec3f &normV, vec2f &texV, int &index) {
	for (int i = 0; i < pos.size(); i++) {
		if (pos[i] == posV) {
			if (i >= norm.size() || norm[i] == normV) {
				if (i >= texCr.size() || texCr[i] == texV) {
					index = i;
					return true;
				}
			}
		}
	}
	return false;
}

void computeTangetBasis(vec3f *vPositions,  vec2f *vUVs, vec3f &tangent, vec3f &binormal)
{
		// Shortcuts for vertices
		vec3f& v0 = vPositions[0];
		vec3f& v1 = vPositions[1];
		vec3f& v2 = vPositions[2];

		// Shortcuts for UVs
		vec2f& uv0 = vUVs[0];
		vec2f& uv1 = vUVs[1];
		vec2f& uv2 = vUVs[2];

		// Edges of the triangle : postion delta
		vec3f deltaPos1 = sub(v1, v0);
		vec3f deltaPos2 = sub(v2, v0);

		// UV delta
		vec2f deltaUV1 = sub(uv1, uv0);
		vec2f deltaUV2 = sub(uv2, uv0);

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		tangent = mult(sub(mult(deltaPos1, deltaUV2.y), mult(deltaPos2, deltaUV1.y)), r);
		binormal = mult(sub(mult(deltaPos2, deltaUV1.x), mult(deltaPos1, deltaUV2.x)), r);
}

void ObjectManager::loadFromOBJ(char * source, std::list<EditorObject*> *outList, int lightningType)
{
	tinyobj::attrib_t objectAttributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	tinyobj::LoadObj(&objectAttributes, &shapes, &materials, &err, source);



	//--------------Shaders-------------------
	char* vShader = FileManager::fileAsCharArray("res/rbrVertTest.glsl");
	char* fShader = FileManager::fileAsCharArray("res/pbrFragTest.glsl");
	GLuint shaders[2];
	if (lightningType == 0)
	{
		shaders[0] = Shader::generateShader(GL_VERTEX_SHADER, vShader);
		shaders[1] = Shader::generateShader(GL_FRAGMENT_SHADER, fShader);
	}
	else if (lightningType == 1)
	{
		shaders[0] = Shader::generateShader(GL_VERTEX_SHADER, FileManager::fileAsCharArray("res/vShaderUI.glsl"));
		shaders[1] = Shader::generateShader(GL_FRAGMENT_SHADER, FileManager::fileAsCharArray("res/fShaderUI.glsl"));
	}
	//shaders[0] = Shader::generateShader(GL_VERTEX_SHADER, FileManager::fileAsCharArray("res/pbrVertShaderGit.glsl"));
	//shaders[1] = Shader::generateShader(GL_FRAGMENT_SHADER, FileManager::fileAsCharArray("res/pdrFragShadetGit.glsl"));
	delete vShader;
	delete fShader;
	//----------------------------------------


	std::list<EditorObject*> objects;

	for (int i = 0; i < shapes.size(); i++) 
	{

		std::vector<vec3f> positions;
		std::vector<vec3f> normals;
		std::vector<vec2f> UVs;

		std::vector<vec3f> binormals;
		std::vector<vec3f> tangents;

		vec3f primitiveTangent;
		vec3f primitiveBinormal;

		std::vector<unsigned> indices;

		positions.reserve(shapes[i].mesh.indices.size());
		normals.reserve(shapes[i].mesh.indices.size());
		UVs.reserve(shapes[i].mesh.indices.size());
		indices.reserve(shapes[i].mesh.indices.size());

		for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
		{
			int positionIndex = shapes[i].mesh.indices[j].vertex_index;
			int normalIndex = shapes[i].mesh.indices[j].normal_index;
			int uvIndex = shapes[i].mesh.indices[j].texcoord_index;

			vec3f vertexPosition;
			vec3f vertexNormal;
			vec2f vertexUV;
			if (positionIndex != -1) {
				vertexPosition = {
					objectAttributes.vertices[positionIndex*3 + 0],
					objectAttributes.vertices[positionIndex*3 + 1],
					objectAttributes.vertices[positionIndex*3 + 2] };
			}

			if (normalIndex != -1) {
				vertexNormal = {
					objectAttributes.normals[normalIndex*3 + 0],
					objectAttributes.normals[normalIndex*3 + 1],
					objectAttributes.normals[normalIndex*3 + 2] };
			}

			if (uvIndex != -1) {
				vertexUV = {
					objectAttributes.texcoords[uvIndex*2 + 0],
					objectAttributes.texcoords[uvIndex*2 + 1] };
			}

			if (j % 3 == 0) {
				vec3f triangP[3];
				triangP[0] = 
					vertexPosition;
				int posInd = shapes[i].mesh.indices[j + 1].vertex_index;
				triangP[1] = {
					objectAttributes.vertices[posInd * 3 + 0],
					objectAttributes.vertices[posInd * 3 + 1],
					objectAttributes.vertices[posInd * 3 + 2]
				};
				posInd = shapes[i].mesh.indices[j + 2].vertex_index;
				triangP[2] = {
					objectAttributes.vertices[posInd * 3 + 0],
					objectAttributes.vertices[posInd * 3 + 1],
					objectAttributes.vertices[posInd * 3 + 2]
				};

				vec2f triangT[3];
				if (uvIndex == -1) {
					triangT[0] = { 0.f, 0.f };
					triangT[1] = { 1.f, 0.f };
					triangT[2] = { 1.f, 1.f };
				}
				else {
					triangT[0] = vertexUV;
					int uvInd = shapes[i].mesh.indices[j+1].texcoord_index;
					triangT[1] = {
						objectAttributes.texcoords[uvInd * 2 + 0],
						objectAttributes.texcoords[uvInd * 2 + 1],
					};
					uvInd = shapes[i].mesh.indices[j + 2].texcoord_index;
					triangT[2] = {
						objectAttributes.texcoords[uvInd * 2 + 0],
						objectAttributes.texcoords[uvInd * 2 + 1],
					};
				}

				computeTangetBasis(triangP, triangT, primitiveTangent, primitiveBinormal);
			}

			int indexToPush;
			if (vertexExist(positions, normals, UVs, vertexPosition, vertexNormal, vertexUV, indexToPush))
			{
				indices.push_back(indexToPush);
				tangents[indexToPush]  = add(tangents[indexToPush], primitiveTangent);
				binormals[indexToPush] = add(binormals[indexToPush], primitiveBinormal);
			}
			else {
				if (positionIndex != -1)
					positions.push_back(vertexPosition);
				if (normalIndex != -1)
					normals.push_back(vertexNormal);
				if (uvIndex != -1)
					UVs.push_back(vertexUV);
				if(!positions.empty())
					indices.push_back(positions.size() - 1);

				tangents.push_back(primitiveTangent);
				binormals.push_back(primitiveBinormal);
			}
			
		}

		tinyobj::material_t material;
		if (shapes[i].mesh.material_ids[0] >= 0)
			material = materials[shapes[i].mesh.material_ids[0]];
		EditorObject *anotherone = EditorObject::Builder()
			.addProgram(Program::generateProgram(shaders, 2))
			.addVPositions(positions)
			.addVNormals(normals)
			.addUVs(UVs)
			.addTangents(tangents)
			.addBinormals(binormals)
			.addIndices(indices)
			.addMaterial(material)
			.build();
			
		outList->push_back(anotherone);
	}

}
