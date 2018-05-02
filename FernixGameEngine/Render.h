#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "Entity.h"
#include "Camera.h"

class Render
{
public:
	Camera camera;
	GLFWwindow* window;

	Render();

	void RenderFrame() {}
	void AddEntity(Entity* entity) {}

	~Render();
private:
	unsigned int SCR_WIDTH = 2000;
	unsigned int SCR_HEIGHT = 1800;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	std::vector<Entity*> entities;
};

