#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "render.h"
#include <vector>

namespace Input {
	glm::vec2 mouse_position = glm::vec2(0.0f, 0.0f);
	float scroll_offset;

	bool firstMouse = true;
	GLFWwindow* window;

	void captureMouse() {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			mouse_position.x = xpos;
			mouse_position.y = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - mouse_position.x;
		float yoffset = mouse_position.y - ypos; // reversed since y-coordinates go from bottom to top

		mouse_position.x = xpos;
		mouse_position.y = ypos;
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		scroll_offset = yoffset;
	}

	bool keyDown(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	void Init(Render render, GLFWwindow* _window) {
		_window = window;

		mouse_position = glm::vec2(render.SCR_WIDTH / 2.0f, render.SCR_HEIGHT / 2.0f);
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetScrollCallback(window, scrollCallback);
	}
}