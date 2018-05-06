#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "render.h"

class Window;

class Input 
{
public:
	glm::vec2 mouse_position = glm::vec2(0.0f, 0.0f);
	glm::vec2 mouse_offset = glm::vec2(0.0f, 0.0f);
	float scroll_offset = 0;

	bool firstMouse = true;

	Window* window;

	void captureMouse(bool);
	bool keyDown(char key);
	bool keyDown(int key);

	Input();
	void ScrollCallback(double xpos, double ypos);
	void CursorPosCallback(double xpos, double ypos);
	void Init(Window* window);

	~Input();
};
