#include "Render.h"
#include "Camera.h"
#include "Entity.h"
#include "Input.h"
#include "Shader.h"
#include <iostream>
#include "metalib/meta.h"
#include "Skybox.h"

DEFTYPE(Render, NULL,
	MEMBER(Render, SCR_WIDTH, uint),
	MEMBER(Render, SCR_HEIGHT, uint),
	MEMBER_NT(Render, camera, POINTER(Camera)),
	MEMBER_NT(Render, skybox, POINTER(Skybox)),
	MEMBER_NT(Render, editor, POINTER(Editor)),
);

//timing
float deltaTime = 0.0f;	// Time between current frame and last frame, might be many render and want the same delta for all 
float lastFrame = 0.0f; // Time of last frame

//render doesnt initialize window

struct Entity;
class Camera;
class Shader;

Render::Render() {};

Render::Render(int SCR_WIDTH, int SCR_HEIGHT)
{
	this->SCR_WIDTH = SCR_WIDTH;
	this->SCR_HEIGHT = SCR_HEIGHT;
}

void Render::Init() {
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
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

void Render::SetLightInfo(Shader shader) {
	if (dirLight) {
		dirLight->setUniforms(0, shader);
	}

	for (unsigned int i = 0; i < pointLights.size(); i++) {
		PointLight* pLight = pointLights[i];
		pLight->setUniforms(i, shader);
	}
	shader.setInt("NR_POINT_LIGHTS", pointLights.size());
}

void Render::SetInGame(bool inGame, Entity* entity) {		
	if (entity == NULL) { entity = level; }
	if (inGame && !this->inGame) {
		for (int i = 0; i < entity->children.size(); i++) {
			entity->children[i]->OnEnterGame();
		}
		entity->OnEnterGame();
	}
	else if (!inGame && this->inGame) {
		for (int i = 0; i < entity->children.size(); i++) {
			entity->children[i]->OnEnterEditor();
		}
		entity->OnEnterEditor();
	}
	this->inGame = inGame;
}

long ticks = 0;
double average = 0;

void updateSceneGraph(Entity* entity) {
	for (unsigned int i = 0; i < entity->children.size(); i++) {
		updateSceneGraph(entity->children[i]);
	}
	entity->Update();
}

void renderSceneGraph(Entity* entity) {
	for (unsigned int i = 0; i < entity->children.size(); i++) {
		renderSceneGraph(entity->children[i]);
	}
	entity->Render();
}

void Render::RenderFrame() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (this->inGame) {
		updateSceneGraph(level);
	}
	else if (this->editor) {
		editor->Update();
		camera->Update();
	}

	renderSceneGraph(level);
}

void Render::SetLevelEntity(Entity* level) {
	this->level = level;
	level->ctx = this;
}

Render::~Render()
{
}
