#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "render.h"

namespace Input {
	glm::vec2 mouse_position = glm::vec2(0.0f, 0.0f);
	float scroll_offset;

	bool firstMouse = true;
	GLFWwindow* window;

	void Init(Render render, GLFWwindow* _window);

	void captureMouse();
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	bool keyDown(int key);
}