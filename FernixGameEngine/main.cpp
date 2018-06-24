#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include "shader.h"

#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Render.h"
#include "Input.h"
#include "Window.h"
#include "AssetManager.h"
#include "Model.h"
#include "Skybox.h" 
#include "Entity.h"
#include "Editor.h"
#include "metalib/meta.h"

int SCR_WIDTH = 3840;
int SCR_HEIGHT = 2160;

const char* title = (char *) "Fernix";

Input input;

Window window(title, &input, SCR_WIDTH, SCR_HEIGHT, true, false);

Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(-3.0f, 0.0f, 0.0f));
AssetManager assetManager;
Render renderer(SCR_WIDTH, SCR_HEIGHT);
std::string saveFolder = "assets/data";

void saveGameState() {

	serializeTyped(renderer.level, saveFolder);
	std::cout << "trying to save game" << std::endl;
}

void gameloop() {
	if (input.keyPressed(GLFW_KEY_S) && input.keyDown(GLFW_KEY_LEFT_CONTROL)) {
		saveGameState();
	}
	renderer.Clear();
	renderer.RenderFrame();
}

int main()
{
	//initialize window
	window.Init();
	input.captureMouse(true);

	Model gun(assetManager.absolute("assets/PISTOL/pistol.obj"));

	Shader ourShader("assets/shaders/pbr.vert", "assets/shaders/pbr.frag");
	Entity cube;
	cube.NewID();

	Entity level;
	level.NewID();

	Skybox skybox;
	skybox.NewID();
	skybox.Init();

	Editor editor;
	editor.NewID();
	editor.selected = &cube;

	renderer.Init();	

	cube.shader = &ourShader;
	cube.model = &gun;
	cube.scale = glm::vec3(0.1);

	camera.NewID();


	PointLight pointLight;
	pointLight.position.y = 3.0f;
	pointLight.position.x = 3.0f;
	pointLight.NewID();

	DirLight dirLight;
	dirLight.rotation = glm::angleAxis(45.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	dirLight.NewID();

	renderer.camera = &camera;
	renderer.skybox = &skybox;
	renderer.editor = &editor;
	renderer.SetDirLight(&dirLight);
	renderer.SetLevelEntity(&level);
	//renderer.AddPointLight(&pointLight);	

	cube.SetParent(&level);
	//renderer.AddEntity(&cube1);
	//renderer.AddEntity(&cube2);
	camera.SetParent(&level);
	pointLight.SetParent(&level);
	skybox.SetParent(&level);
	
	editor.SetParent(&level);

	window.gameLoop(gameloop);

	saveGameState();

	return 0;
}



