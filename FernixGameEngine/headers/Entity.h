#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <vector>
#include "metalib/meta.h"

class Shader;
class Render;
class Camera;
class Model;

struct Entity : Typed {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(0.0f,0.0f,0.0f,0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Model* model;
	Shader* shader;
	Render* ctx;
	Entity* parent;
	std::vector<Entity*> children;

	virtual void Init() {};
	virtual void Update();
	virtual void Render();
	virtual void OnEnterEditor();
	virtual void OnEnterGame();

	glm::mat4 ModelMatrix();

	bool RunningGame();

	Entity(Model* model, Shader* shader);
	Entity();
	void SetShaderProps(Shader shader);
	virtual ~Entity();
};

extern StructType* EntityType;
extern StructType* glm_vec3Type;

