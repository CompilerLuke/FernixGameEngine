#include "Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Render.h"
#include "Model.h"
#include "Skybox.h"
#include <iostream>

class Render;
class Camera;

glm::mat4 Transform::ModelMatrix() {
	glm::mat4 matrix = glm::mat4(1.0f);

	matrix = glm::translate(matrix, position);
	matrix *= glm::mat4_cast(rotation);
	matrix = glm::scale(matrix, scale);


	//std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;

	return matrix; //check if this doesnt cause some kind of scope error
}

Entity::Entity(Transform transform, Model* model, Shader* shader)
{
	transform = transform;
	model = model;
	shader = shader;
}

Entity::Entity() {
	transform = Transform();
	model = NULL;
	shader = NULL;
}

void Entity::SetShaderProps(Shader shader) {
	Camera* camera = ctx->camera;
	Skybox* skybox = ctx->skybox;

	glm::mat4 view = ctx->camera->GetViewMatrix();
	shader.setMat4("view", view);

	glm::mat4 model = this->transform.ModelMatrix();
	glm::mat3 normalModel = glm::transpose(glm::inverse(model));

	shader.setMat4("model", model);
	shader.setMat3("normalModel", normalModel);
	shader.setVec3("viewPos", camera->transform.position);
	shader.setMat4("projection", camera->GetProjectionMatrix());

	ctx->SetLightInfo(shader);
}

void Entity::Update() {};
void Entity::Render() {
	if (shader && model) {
		shader->use();
		this->SetShaderProps(*shader);
		model->Render(*shader, ctx->skybox);
	}
};

Entity::~Entity()
{
}
