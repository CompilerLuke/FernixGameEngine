#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include "Render.h"

class Transform {
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix();
};

class Entity
{
public:
	float deltaTime;
	Transform transform;

	virtual void mouse_callback(GLFWwindow *window, double xpos, double ypos);
	virtual void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
	virtual void processInput(GLFWwindow *window);
	virtual void update(float deltaTime);

private:
	Entity(Transform transform);
	Entity();
	~Entity();
};

