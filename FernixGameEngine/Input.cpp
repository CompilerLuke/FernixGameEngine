#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "render.h"
#include <vector>
#include "Input.h"
#include <iostream>

void Input::captureMouse() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Input::Input() {}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Input* self = (Input*)glfwGetWindowUserPointer(window);
	
	if (self->firstMouse)
	{
		self->mouse_position.x = xpos;
		self->mouse_position.y = ypos;
		self->firstMouse = false;
	}

	float xoffset = xpos - self->mouse_position.x;
	float yoffset = self->mouse_position.y - ypos; // reversed since y-coordinates go from bottom to top

	self->mouse_offset.x += xoffset;
	self->mouse_offset.y += yoffset;

	self->mouse_position.x = xpos;
	self->mouse_position.y = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input* self = (Input*)glfwGetWindowUserPointer(window);
	self->scroll_offset = yoffset;
}

bool Input::keyDown(char keyS) {
	int key = glfwGetKey(window, keyS);
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::keyDown(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

Input::Input(Render render, GLFWwindow* _window) {
	this->Init(render, _window);
}

void Input::Init(Render render, GLFWwindow* _window) {
	window = _window;

	mouse_position = glm::vec2(render.SCR_WIDTH / 2.0f, render.SCR_HEIGHT / 2.0f);

	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

Input::~Input() {}
