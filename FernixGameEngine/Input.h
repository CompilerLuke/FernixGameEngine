#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "render.h"

class Input 
{
public:
	glm::vec2 mouse_position = glm::vec2(0.0f, 0.0f);
	glm::vec2 mouse_offset = glm::vec2(0.0f, 0.0f);
	float scroll_offset = 0;

	bool firstMouse = true;
	GLFWwindow* window;

	void captureMouse();
	bool keyDown(char key);
	bool keyDown(int key);

	Input(Render render, GLFWwindow* _window);
	void Init(Render render, GLFWwindow* _window);
	Input();
	~Input();
};
