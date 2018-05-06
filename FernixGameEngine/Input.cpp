#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "render.h"
#include <vector>
#include "Input.h"
#include "Window.h"
#include <iostream>

void Input::captureMouse(bool capture) {
	window->captureMouse(capture);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Input::Input() {}

void Input::CursorPosCallback(double xpos, double ypos)
{
	if (this->firstMouse)
	{
		this->mouse_position.x = xpos;
		this->mouse_position.y = ypos;
		this->firstMouse = false;
	}

	float xoffset = xpos - this->mouse_position.x;
	float yoffset = this->mouse_position.y - ypos; // reversed since y-coordinates go from bottom to top

	this->mouse_offset.x += xoffset;
	this->mouse_offset.y += yoffset;

	this->mouse_position.x = xpos;
	this->mouse_position.y = ypos;
}

void Input::ScrollCallback(double xoffset, double yoffset)
{
	this->scroll_offset = yoffset;
}

bool Input::keyDown(char keyS) {
	return window->getKeyDown(keyS);
}

bool Input::keyDown(int key) {
	return window->getKeyDown(key);
}

void Input::Init(Window* window) {
	this->window = window;
	mouse_position = glm::vec2(window->SCR_WIDTH / 2.0f, window->SCR_HEIGHT / 2.0f);

	//glfwSetWindowUserPointer(window, this);
	//glfwSetCursorPosCallback(window, mouseCallback);
	//glfwSetScrollCallback(window, scrollCallback);
}

Input::~Input() {}
