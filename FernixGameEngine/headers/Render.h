#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "Entity.h"
#include "Light.h"
#include "Editor.h"

struct Entity;
class Camera;
class Light;
class PointLight;
class DirLight;
class Skybox;
struct Editor;

class Render
{
public:
	unsigned int SCR_WIDTH = 2000;
	unsigned int SCR_HEIGHT = 1800;

	Camera* camera;
	Skybox* skybox;
	Editor* editor;
	GLFWwindow* window;

	float deltaTime = 0.0f;

	bool inGame = true;

	Render(int,int);

	void RenderFrame();
	void Clear();
	void AddEntity(Entity *entity);
	void AddPointLight(PointLight *light);
	void SetDirLight(DirLight* light);
	void SetLightInfo(Shader s);
	void SetInGame(bool inGame);
	void Init();

	~Render();
private:
	float lastFrame = 0.0f;

	std::vector<Entity*> entities;
	std::vector<PointLight*> pointLights;
	DirLight* dirLight;
};

