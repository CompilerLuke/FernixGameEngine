#include "Render.h"
#include "Camera.h"
#include "Entity.h"
#include "Input.h"
#include "Shader.h"
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
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
}

void Render::Clear() {
	glClearColor(0.2, 0.3f, 0.3, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::AddPointLight(PointLight* light) {
	pointLights.push_back(light);
}

void Render::SetDirLight(DirLight* dirLight) {
	this->dirLight = dirLight;
}

void Render::SetLightInfo(Shader& shader) {
	dirLight->setUniforms(0, shader);

	for (unsigned int i = 0; i < pointLights.size(); i++) {
		PointLight* pLight = pointLights[i];
		pLight->setUniforms(i, shader);
	}
	shader.setUInt("NR_POINT_LIGHTS", pointLights.size());
}

void Render::RenderFrame() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->Update();
	}

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}
}

void Render::AddEntity(Entity* entity) { //assume entity is heap allocated, and will be deleted
	entity->ctx = this;
	entities.push_back(entity);
}

Render::~Render()
{
}
