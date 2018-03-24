#ifndef SIMP_UI
#define SIMP_UI

#include "FileManager.h"
#include "Program.h"
#include "Shader.h"
#include "GLObject.h"
#include "Texture.h"
#include "Button.h"
#include <list>
#include "GLBuffer.h"

class SimpleUI
{
public:
	class Builder {
		public:
			vec2f size;
			vec2f pos;
			std::list<Button*> *buttons;
			Builder(vec2f  _pos, vec2f _size);
			Builder& addButton(Button *item);
			SimpleUI* buildUI();
	};
	~SimpleUI();
	void draw();
	void actionHandle();
private:
	
	//to prevent non builder initialisation
	SimpleUI() = delete;

	SimpleUI(Builder* bilder);
	std::list<Button*> *buttons;
	GLuint* elemsTextures;

	GLuint vao;
	GLuint program;

	vec2f size;
	vec2f pos;
};

#endif