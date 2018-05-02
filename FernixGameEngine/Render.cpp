#include "Render.h"
#include "Camera.h"
#include "Entity.h"

//timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Render::Render()
{
	const unsigned int SCR_WIDTH = 2000;
	const unsigned int SCR_HEIGHT = 1800;

	//Camera
	Camera camera;
}



void Render::RenderFrame() {
	glm::mat4 projection = camera.GetProjectionMatrix();
	glm::mat4 view = camera.GetViewMatrix();

	for (int i = 0; i < entities.size; i++) {
		entities[i]->processInput(window);
	}
}

void Render::AddEntity(Entity* entity) { //assume entity is heap allocated, and will be deleted
	entities.push_back(entity);
}


Render::~Render()
{
}
