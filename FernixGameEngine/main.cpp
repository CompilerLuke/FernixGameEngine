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
#include "Spatial.h"
#include "Model.h"

int SCR_WIDTH = 3840;
int SCR_HEIGHT = 2160;

const char* title = (char *) "Fernix";

Input input;

Window window(title, &input, SCR_WIDTH, SCR_HEIGHT, true, false);
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(-3.0f, 0.0f, 0.0f));
AssetManager assetManager;
Render renderer(SCR_WIDTH, SCR_HEIGHT);

class Cube : public Spatial {
public:
	Resource texture;

	Cube(Model* model, Shader* shader) : Spatial(model, shader) {
		std::cout << shader << std::endl;
		assetManager.loadTexture("stylzed castle texture.jpg");
		Option option = assetManager.loadTexture("stylzed castle texture.jpg");

		if (option.error != "") {
			std::cout << option.error << std::endl;
		}
		texture = option.resource;
		std::cout << texture.ID << std::endl;
	}

	void Render() override {
		Spatial::Render();
	}

	~Cube() {
	}
};

void gameloop() {
	renderer.Clear();
	renderer.RenderFrame();
}

int main()
{
	//initialize window
	window.Init();
	input.captureMouse(true);

	renderer.Init();

	Model gun(assetManager.absolute("PISTOL/pistol.obj"));

	Shader ourShader("pbr.vert", "pbr.frag");
	Cube cube(&gun, &ourShader);

	//cube.transform.rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	cube.transform.scale = glm::vec3(0.001f);
	cube.shader = &ourShader;
	cube.model = &gun;
	
	//PointLight pointLight;
	//pointLight.transform.position.y = 3.0f;
	//pointLight.transform.position.x = 3.0f;

	DirLight dirLight;
	dirLight.transform.rotation = glm::angleAxis(45.0f, glm::vec3(1.0f, 1.0f, 0.0f));

	renderer.camera = &camera;
	renderer.SetDirLight(&dirLight);
	renderer.AddEntity(&cube);
	renderer.AddEntity(&camera);
	renderer.AddEntity(&dirLight);

	window.gameLoop(gameloop);

	return 0;
}



