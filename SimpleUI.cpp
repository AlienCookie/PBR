#include "stdafx.h"
#include "SimpleUI.h"

SimpleUI::SimpleUI(SimpleUI::Builder* builder)
{
	buttons = builder->buttons;

	size = builder->size;
	pos = builder->pos;


	if (buttons->empty())
		return;

	char* vShaderText;
	char* fShaderText;

	/*const int indicesNumber = 6;

	const int verticesNumber = 4;

	float vr[verticesNumber*vertexSize] =
	{	//| vertex position                 |  uvCoor
		pos[0],         pos[1], 0.f,		  0.f, 1.f ,
		pos[0]+size[0],	pos[1], 0.f,		  1.f, 1.f,
		pos[0]+size[0], pos[1]-size[1], 0.f,  1.f, 0.f,
		pos[0],			pos[1]-size[1], 0.f,  0.f, 0.f,
	};

	int indices[indicesNumber] = {
		0, 1, 2,
		2, 3, 0
	};*/

	vShaderText = FileManager::fileAsCharArray("res/vShaderUI.glsl");
	fShaderText = FileManager::fileAsCharArray("res/fShaderUI.glsl");

	GLuint shaders[2];
	shaders[0] = Shader::generateShader(GL_VERTEX_SHADER, vShaderText);
	shaders[1] = Shader::generateShader(GL_FRAGMENT_SHADER, fShaderText);

	program = Program::generateProgram(shaders, 2);

	glGenVertexArrays(1, &vao);

	glUseProgram(program);

	glBindVertexArray(vao);




	int elemsLen = buttons->size();
	elemsTextures = new GLuint[elemsLen];

	const int vertexSize = 5;

	auto item = buttons->begin();
	int singleElemLen = (*item)->getVerticesDataLen();

	int vrLen = elemsLen * singleElemLen;
	float *vr = new float[vrLen];

	int indLen = 6 * elemsLen;
	int *indices = new int[indLen];

	for (int i = 0; item != buttons->end(); item++, i++)
	{
		float* itemData = (*item)->getVerticesData();
		std::copy(itemData, itemData + singleElemLen, vr + i*singleElemLen);

		indices[i * 6] = i * 4;     
		indices[i * 6 + 1] = i * 4 + 1; 
		indices[i * 6 + 2] = i * 4 + 2; 
		
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4;

		elemsTextures[i] = Texture::loadTexturePNG((*item)->getTextureSource());
	}


	GLBuffer* buffer = new GLBuffer(vr, vrLen * sizeof(float));
	GLBuffer* elBuffer = new GLBuffer(indices, indLen * sizeof(int), GL_ELEMENT_ARRAY_BUFFER);

	glBindBuffer(buffer->getTarget(), buffer->getId());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)12);

	
	glUniform1i(glGetUniformLocation(program, "gSampler"), 7);

	delete indices;
	delete buffer;
	delete elBuffer;
	delete vShaderText;
	delete fShaderText;
	delete vr;
}

void SimpleUI::draw()
{
	glDisable(GL_DEPTH_TEST);
	int i = 0;

	for (auto item = buttons->begin(); item != buttons->end(); item++, i++)
	{

		glUseProgram(program);

		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0 + 7);
		glBindTexture(GL_TEXTURE_2D, elemsTextures[i]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((6*i)*sizeof(int)));
	}
	glEnable(GL_DEPTH_TEST);
}

void SimpleUI::actionHandle() {
	if (mouseState.left_button == UP &&
		mouseState.right_button == UP &&
		mouseState.midle_button == UP)
		return;
	float x = (float)((mouseState.clickPositionX * 2. - mouseState.windowWidth)/ mouseState.windowWidth);
	float y = (float)((mouseState.clickPositionY * 2. - mouseState.windowHeight) / -mouseState.windowHeight);

	for (auto item = buttons->begin(); item != buttons->end(); item++)
		(*item)->checkInteraction(x, y);

	mouseState.clickPositionX = -9000.;
	mouseState.clickPositionY = -9000.;

}

SimpleUI::~SimpleUI()
{
	//for (auto bt = buttons->begin(); bt != buttons->end(); bt++)
	//	delete (*bt);
	delete buttons;
	delete elemsTextures;

}

SimpleUI::Builder::Builder(vec2f _pos, vec2f _size)
{
	size = _size;
	pos = _pos;

	buttons = new std::list<Button*>();
}

SimpleUI::Builder& SimpleUI::Builder::addButton(Button *item)
{
	buttons->push_back(item);
	return *this;
}

SimpleUI* SimpleUI::Builder::buildUI()
{
	return new SimpleUI(this);
}
