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

int SCR_WIDTH = 3840;
int SCR_HEIGHT = 2160;

const char* title = (char *) "Fernix";

Input input;

Window window(title, &input, SCR_WIDTH, SCR_HEIGHT, false, false);
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(-3.0f, 0.0f, 0.0f));
AssetManager assetManager;
Render renderer(SCR_WIDTH, SCR_HEIGHT);

void gameloop() {
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
	Entity cube1;
	Entity cube2;

	Skybox skybox;
	skybox.Init();

	Editor editor;
	editor.selected = &cube;

	renderer.Init();	

	//cube.transform.rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//cube.transform.scale = glm::vec3(0.001f);
	cube.shader = &ourShader;
	cube.model = &gun;
	cube.scale = glm::vec3(0.1);


	PointLight pointLight;
	pointLight.position.y = 3.0f;
	pointLight.position.x = 3.0f;

	DirLight dirLight;
	dirLight.rotation = glm::angleAxis(45.0f, glm::vec3(1.0f, 1.0f, 0.0f));

	renderer.camera = &camera;
	renderer.skybox = &skybox;
	renderer.editor = &editor;
	renderer.SetDirLight(&dirLight);
	//renderer.AddPointLight(&pointLight);	

	renderer.AddEntity(&cube);
	//renderer.AddEntity(&cube1);
	//renderer.AddEntity(&cube2);
	renderer.AddEntity(&camera);
	renderer.AddEntity(&pointLight);
	renderer.AddEntity(&skybox);	
	
	renderer.AddEntity(&editor);

	window.gameLoop(gameloop);

	return 0;
}



