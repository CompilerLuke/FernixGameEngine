#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

class Shader;
class Render;
class Camera;

class Transform {
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(0.0f,0.0f,0.0f,0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 ModelMatrix();
};

class Entity
{
public:
	Transform transform;
	Render* ctx;

	virtual void Update();
	virtual void Render();

	Entity(Transform transform);
	Entity();
	void SetShaderProps(Shader shader);
	virtual ~Entity();
};

