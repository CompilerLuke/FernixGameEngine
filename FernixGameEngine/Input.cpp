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

glm::vec2 Input::toScreenPos(double xpos, double ypos) {
	return glm::vec2(
		((xpos / window->SCR_WIDTH * 2) - 1),
		-((ypos / window->SCR_HEIGHT * 2) - 1)
	);
}

void Input::ScrollCallback(double xoffset, double yoffset)
{
	this->scroll_offset = yoffset;
}

void Input::KeyCallback(int key, int action) {
	keys[key] = action;
}

bool Input::keyPressed(int keyS) {
	if (last_frame_keys.find(keyS) == last_frame_keys.end()) {
		return keyDown(keyS);
	}
	return last_frame_keys[keyS] == 0 && keyDown(keyS);
}

bool Input::rightClicking() {
	return window->getMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT);
}

bool Input::leftClicking() {
	return window->getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT);
}

void Input::setMousePos(double xpos, double ypos) {
	window->setMousePos(xpos, ypos);
}

bool Input::keyDown(int key) {
	return window->getKeyDown(key);
}

void Input::Clear() {
	for (std::pair<int, int> element : keys)
	{
		last_frame_keys[element.first] = element.second;
	}

	keys.clear();
}	

void Input::Init(Window* window) {
	this->window = window;
	mouse_position = glm::vec2(window->SCR_WIDTH / 2.0f, window->SCR_HEIGHT / 2.0f);

	//glfwSetWindowUserPointer(window, this);
	//glfwSetCursorPosCallback(window, mouseCallback);
	//glfwSetScrollCallback(window, scrollCallback);
}

Input::~Input() {}
