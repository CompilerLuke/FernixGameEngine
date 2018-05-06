#include "Render.h"
#include "Camera.h"
#include "Entity.h"
#include "Input.h"
#include <iostream>

//timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//render doesnt initialize window

class Entity;
class Camera;

Render::Render(int SCR_WIDTH, int SCR_HEIGHT)
{
	this->SCR_WIDTH = SCR_WIDTH;
	this->SCR_HEIGHT = SCR_HEIGHT;
}

void Render::Init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void Render::Clear() {
	glClearColor(0.2, 0.3f, 0.3, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::RenderFrame() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->update(deltaTime);
	}

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->render(*camera);
	}
}

void Render::AddEntity(Entity* entity) { //assume entity is heap allocated, and will be deleted
	entities.push_back(entity);
}

Render::~Render()
{
}
