#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer() {
	glClearColor(0.0f, 0.3f, 0.3f, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

Renderer::~Renderer() {

}

void Renderer::render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto object = objectsList.begin();
		object != objectsList.end(); object++)
	{
		if ((*object)->isOnDraw()) {
			glUseProgram((*object)->getProgram());

			glBindVertexArray((*object)->getVAO());

			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, (*object)->getAlbedoMap());

			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, (*object)->getNormalMap());

			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, (*object)->getMetallicMap());

			glActiveTexture(GL_TEXTURE0 + 3);
			glBindTexture(GL_TEXTURE_2D, (*object)->getRoughnessMap());

			glActiveTexture(GL_TEXTURE0 + 4);
			glBindTexture(GL_TEXTURE_2D, (*object)->getAoMap());

			glActiveTexture(GL_TEXTURE0 + 5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, (*object)->getCubemap());

			(*object)->updateMVP();

			glDrawElements((*object)->getMode(), (*object)->getCount(), GL_UNSIGNED_INT, (void*)(*object)->getFirst());
		}

	}
	glBindVertexArray(0);

}

void Renderer::addObject(GLObject *object) {
	objectsList.push_back(object);
}

void Renderer::addAllObjects(std::list<GLObject*> *objects)
{
	objectsList.merge(*objects);
}

void Renderer::replaceAllObjects(std::list<GLObject*> *objects)
{
	objectsList.clear();
	addAllObjects(objects);
}