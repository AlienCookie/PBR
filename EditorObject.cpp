#include "stdafx.h"
#include "EditorObject.h"
#include <iostream>

std::vector<vec3f> EditorObject::getVertPositions()
{
	return std::vector<vec3f>();
}

std::vector<unsigned> EditorObject::getIndeces()
{
	return std::vector<unsigned>();
}

EditorObject::EditorObject(EditorObject::Builder* builder) :
	GLObject(builder->objectVAO, builder->objectProgram, builder->indices.size())
{

	vPositions = builder->vPositions;
	indices = builder->indices;
	material = builder->material;

	setAlbedoMap(builder->albedoMap);
	setNormalMap(builder->normalMap);
	setMetallicMap(builder->metallicMap);
	setRoughnessMap(builder->roughnessMap);
	setAoMap(builder->aoMap);
	setCubemap(builder->cubeMap);
}


EditorObject::~EditorObject()
{
	glDeleteProgram(getProgram());
}

EditorObject::Builder::Builder()
{
	// setting default values
	objectVAO = 0;
	objectProgram = -1;
	albedoMap = -1;
}

EditorObject::Builder& EditorObject::Builder::addProgram(GLuint _program)
{
	objectProgram = _program;
	return *this;
}

EditorObject::Builder& EditorObject::Builder::addVAO(GLuint _VAO)
{
	objectVAO = _VAO;
	return *this;
}

EditorObject::Builder & EditorObject::Builder::addVPositions(std::vector<float> positions)
{
	int vertSize = positions.size() / 3;
	vPositions.reserve(vertSize);
	for (int i = 0; i < vertSize; i++)
	{
		vec3f vertex{ positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2] };
		vPositions.push_back(vertex);
	}
	return *this;
}

EditorObject::Builder & EditorObject::Builder::addVPositions(std::vector<vec3f> positions)
{
	vPositions = positions;
	return *this;
}

EditorObject::Builder & EditorObject::Builder::addVNormals(std::vector<float> normals)
{
	int vertSize = normals.size() / 3;
	vNormals.reserve(vertSize);

	for (int i = 0; i < vertSize; i++)
	{
		vec3f vertex{ normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2] };
		vNormals.push_back(vertex);
	}
	return *this;
}

EditorObject::Builder & EditorObject::Builder::addVNormals(std::vector<vec3f> normals)
{
	vNormals = normals;
	return *this;
}

EditorObject::Builder & EditorObject::Builder::addUVs(std::vector<float> uvs)
{
	int vertSize = uvs.size() / 2;
	vUVs.reserve(vertSize);

	for (int i = 0; i < vertSize; i++)
	{
		vec2f uv{ uvs[i * 3], uvs[i * 3 + 1]};
		vUVs.push_back(uv);
	}
	return *this;
}


EditorObject::Builder & EditorObject::Builder::addUVs(std::vector<vec2f> uvs)
{
	vUVs = uvs;
	return *this;
}

EditorObject::Builder & EditorObject::Builder::addTangents(std::vector<vec3f> tngnts)
{
	vTangents = tngnts;

	return *this;
}

EditorObject::Builder & EditorObject::Builder::addBinormals(std::vector<vec3f> _binormals)
{
	vBinormals = _binormals;

	return *this;
}

EditorObject::Builder & EditorObject::Builder::addIndices(std::vector<unsigned> _indices)
{
	indices = _indices;
	return *this;
}

EditorObject::Builder & EditorObject::Builder::addMaterial(tinyobj::material_t objectMaterial)
{
	material = objectMaterial;
	return *this;
}



EditorObject* EditorObject::Builder::build()
{


	int positionsDataPointer = 0;
	int uvsDataPointer = (positionsDataPointer + vPositions.size()*sizeof(vec3f));
	int normalsDataPointer = (uvsDataPointer + vUVs.size()*sizeof(vec2f));
	int tangentsDataPointer = (normalsDataPointer + vNormals.size() * sizeof(vec3f));
	int binormalsDataPointer = (tangentsDataPointer + vTangents.size()*sizeof(vec3f));
	int dataSize = (binormalsDataPointer + vBinormals.size()*sizeof(vec3f));

	char *verticesData = new char[dataSize];
	if (!vPositions.empty())
		std::memcpy(verticesData + positionsDataPointer, &vPositions.front(), vPositions.size()*sizeof(vec3f));
	if (!vUVs.empty())
		std::memcpy(verticesData + uvsDataPointer, &vUVs.front(), vUVs.size()*sizeof(vec2f));
	if (!vNormals.empty())
		std::memcpy(verticesData + normalsDataPointer, &vNormals.front(), vNormals.size()*sizeof(vec3f));
	if (!vTangents.empty())
		std::memcpy(verticesData + tangentsDataPointer, &vTangents.front(), vTangents.size()*sizeof(vec3f));
	if (!vBinormals.empty())
		std::memcpy(verticesData + binormalsDataPointer, &vBinormals.front(), vBinormals.size()*sizeof(vec3f));

	GLBuffer verticesDataBuffer =
		GLBuffer(verticesData, dataSize);

	glUseProgram(objectProgram);

	glGenVertexArrays(1, &objectVAO);
	glBindVertexArray(objectVAO);

	glBindBuffer(verticesDataBuffer.getTarget(), verticesDataBuffer.getId());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)positionsDataPointer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)uvsDataPointer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)normalsDataPointer);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)tangentsDataPointer);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)binormalsDataPointer);


	GLBuffer(&indices.front(), sizeof(int)*indices.size(), GL_ELEMENT_ARRAY_BUFFER);



	/*if (material.diffuse_texname != "")
	{
		objectTexture = Texture::loadTexturePNG(material.diffuse_texname.c_str());
	}

	glUniform1i(glGetUniformLocation(objectProgram, "map_Kd"), 0);

	if (material.bump_texname != "")
	{
		normalTexture = Texture::loadTexturePNG(material.bump_texname.c_str());
	}
	glUniform1i(glGetUniformLocation(objectProgram, "map_Bump"), 1);

	glUniform3f(glGetUniformLocation(objectProgram, "mambient"), material.ambient[0], material.ambient[1], material.ambient[2]);      //setting the material property
	glUniform3f(glGetUniformLocation(objectProgram, "mdiffuse"), material.diffuse[0], material.diffuse[1], material.diffuse[2]);
	glUniform3f(glGetUniformLocation(objectProgram, "mspecular"), material.specular[0], material.specular[1], material.specular[2]);
	glUniform3f(glGetUniformLocation(objectProgram, "transmittance"), material.transmittance[0], material.transmittance[1], material.transmittance[2]);
	glUniform1f(glGetUniformLocation(objectProgram, "shininess"), material.shininess);    //shininess

	glUniform3f(glGetUniformLocation(objectProgram, "lambient"), 0.2, 0.2, 0.2);      //setting light property
	glUniform3f(glGetUniformLocation(objectProgram, "ldiffuse"), 0.6, 0.6, 0.6);
	glUniform3f(glGetUniformLocation(objectProgram, "lspecular"), 1.0, 1.0, 1.0);
	glUniform3f(glGetUniformLocation(objectProgram, "lightPos"), 0.0, 0.0, -3.0);*/

	/*glUniform3fv(glGetUniformLocation(objectProgram, "albedo"), 1, material.diffuse);
	glUniform1f(glGetUniformLocation(objectProgram, "metallic"), material.metallic);
	glUniform1f(glGetUniformLocation(objectProgram, "roughness"), material.roughness);
	glUniform1f(glGetUniformLocation(objectProgram, "ao"), material.ior);*/

	if (material.diffuse_texname != "")
	{
		albedoMap = Texture::loadTexturePNG(material.diffuse_texname.c_str());
	}

	glUniform1i(glGetUniformLocation(objectProgram, "albedoMap"), 0);

	if (material.normal_texname != "")
	{
		normalMap = Texture::loadTexturePNG(material.normal_texname.c_str());
	}
	glUniform1i(glGetUniformLocation(objectProgram, "normalMap"), 1);

	if (material.metallic_texname != "")
	{
		metallicMap = Texture::loadTexturePNG(material.metallic_texname.c_str());
	}
	glUniform1i(glGetUniformLocation(objectProgram, "metallicMap"), 2);

	if (material.roughness_texname != "")
	{
		roughnessMap = Texture::loadTexturePNG(material.roughness_texname.c_str());
	}
	glUniform1i(glGetUniformLocation(objectProgram, "roughnessMap"), 3);

	if (material.displacement_texname != "")
	{
		aoMap = Texture::loadTexturePNG(material.displacement_texname.c_str());
	}
	glUniform1i(glGetUniformLocation(objectProgram, "aoMap"), 4);


	vec3f lightPos[7] = {
			{ 0.0f, 0.0f, 0.0f  },
			{ -0.5f, 0.0f, -0.0f  },
			{ -0.5f, -0.5f, -0.5f },
			{ 0.4f, -0.4f, 0.5f  },
			{ -0.3f, -0.3f, 0.8f },
			{ -0.37f, -0.28f, 0.8f},
			{ -0.4f, -0.3f, 0.8f },};

	vec3f lightCol[7] = {
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f }, };

	for (int i = 0; i < 7; i++)
	{
		glUniform3fv(glGetUniformLocation(objectProgram, (std::string("lightPositions[") + std::to_string(i) + std::string("]")).c_str()), 1, &lightPos[i].x);
		glUniform3fv(glGetUniformLocation(objectProgram, (std::string("lightColors[") + std::to_string(i) + std::string("]")).c_str()), 1, &lightCol[i].x);
	}

	vec3f camPos = { 0.0f, 0.0f, -1.0f };
	glUniform3fv(glGetUniformLocation(objectProgram, "camPos"), 1, &camPos.x);


	std::vector<const char*> faces;
	faces.push_back("res/cubemap/right.png");
	faces.push_back("res/cubemap/left.png");
	faces.push_back("res/cubemap/top.png");
	faces.push_back("res/cubemap/bottom.png");
	faces.push_back("res/cubemap/back.png");
	faces.push_back("res/cubemap/front.png");

	cubeMap = Texture::loadTextureCubemap(faces);
	glUniform1i(glGetUniformLocation(objectProgram, "skybox"), 5);

	return new EditorObject(this);
}


