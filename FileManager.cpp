#include "stdafx.h"
#include "FileManager.h"

#include <iostream>

char* FileManager::fileAsCharArray(std::string source)
{
	std::ifstream t(source);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return _strdup(str.c_str());
}

bool FileManager::loadOBJ(
	const char * path, 
	std::vector<vec3f> & out_vertices,
	std::vector<vec2f> & out_uvs, 
	std::vector<vec3f> & out_normals,
	std::vector<unsigned> & indices,
	unsigned int *divise)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3f> temp_vertices;
	std::vector<vec2f> temp_uvs;
	std::vector<vec3f> temp_normals;

	std::vector<unsigned int> division;
	unsigned int tmp = 0;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		std::cerr << "Incorrect path!\n";
		getchar();
		return false;
	}

	while (true)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			vec3f vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2f uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			// Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3f normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

			if (divise)
				tmp += 3;
		}
		else
			if (divise)
				if (strcmp(lineHeader, "usemtl") == 0)
				{
					division.push_back(tmp);
				}


				else {
					// Probably a comment, eat up the rest of the line
					char stupidBuffer[1000];
					fgets(stupidBuffer, 1000, file);
				}

	}
	out_vertices.reserve(vertexIndices.size() * 3);
	out_uvs.reserve(uvIndices.size() * 2);
	out_normals.reserve(normalIndices.size() * 3);
	indices.reserve(vertexIndices.size());

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		vec3f vertex { temp_vertices[vertexIndex - 1].x, temp_vertices[vertexIndex - 1].y, temp_vertices[vertexIndex - 1].z };
		vec2f uv { temp_uvs[uvIndex - 1].x, temp_uvs[uvIndex - 1].y};
		vec3f normal { temp_normals[normalIndex - 1].x, temp_normals[normalIndex - 1].y, temp_normals[normalIndex - 1].z };

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
		indices.push_back(i);

	}
	if (divise) {
		division.push_back(vertexIndices.size());
		for (int i = 0; i<division.size(); i++)
			divise[i] = division[i];
	}
	return true;
}
