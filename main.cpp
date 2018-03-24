// PBR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GLFWWindow.h"
#include "Renderer.h"
#include "FileManager.h"
#include "EditorObject.h"
#include "ObjectManager.h"
#include "ActionManager.h"
#include "ActiveObjectAction.h"
#include "Button.h"
#include "SimpleUI.h"
#include <vld.h>

int main()
{
	GLFWWindow window(1280, 720);
	Renderer renderer;


	std::list<EditorObject*> items;

	ObjectManager::loadFromOBJ("res/sphere1.obj", &items, 0);
	ObjectResetAction action1(items.back());

	ObjectManager::loadFromOBJ("res/sphere2.obj", &items, 0);
	ObjectResetAction action2(items.back());

	ObjectManager::loadFromOBJ("res/sphere3.obj", &items, 0);
	ObjectResetAction action3(items.back());

	Button button1(-0.3f, -0.7f, 0.13f, 0.2f, "res/sButton1.png");
	Button button2(-0.0f, -0.7f, 0.13f, 0.2f, "res/sButton2.png");
	Button button3(0.3f, -0.7f, 0.13f, 0.2f, "res/sButton3.png");

	button1.setAction(&action1);
	button2.setAction(&action2);
	button3.setAction(&action3);

	vec2f pos{ 0.0f, 0.0f };
	vec2f size{ 0.3f, 0.3f };

	SimpleUI *simpleUI = SimpleUI::
		Builder(pos, size)
		.addButton(&button1)
		.addButton(&button2)
		.addButton(&button3)
		.buildUI();

	ActionManager actionManager(items);

	//ObjectManager::loadFromOBJ("res/cubebox.obj", &items, 1);

	for (auto obj = items.begin(); obj != items.end(); obj++) {
	//	if (obj == items.begin())
		//	(*obj)->setOnDraw(true);
		//else
		(*obj)->setOnDraw(false);
		//(*obj)->scale(vec3f{4.0,4.0,4.0});
		//(*obj)->translate(vec3f{0.0, 0.0, -4.0});
		renderer.addObject((*obj));
	}

//	ActionManager actionManager(items);

	while (!window.shouldClose())
	{
		window.pollEvents();

		renderer.render();

		actionManager.mouseHandle();

		simpleUI->actionHandle();
		simpleUI->draw();

		window.swapBuffers();
	}
	for (auto item = items.begin(); item != items.end(); item++)
		delete (*item);
	delete simpleUI;

	return EXIT_SUCCESS;
}

