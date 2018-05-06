#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "Entity.h"

class Entity;
class Camera;

class Render
{
public:
	unsigned int SCR_WIDTH = 2000;
	unsigned int SCR_HEIGHT = 1800;

	Camera* camera;
	GLFWwindow* window;

	Render(int,int);

	void RenderFrame();
	void Clear();
	void AddEntity(Entity *entity);
	void Init();

	~Render();
private:
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	std::vector<Entity*> entities;
};

