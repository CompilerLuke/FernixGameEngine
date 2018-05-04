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

	virtual void update(float deltaTime);
	virtual void render(Camera& camera);

	Entity(Transform transform);
	Entity();
	void SetShaderProps(Shader& shader, Camera& camera);
	virtual ~Entity();
};

