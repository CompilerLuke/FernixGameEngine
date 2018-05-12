#include "Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Render.h"
#include <iostream>

glm::mat4 Transform::ModelMatrix() {
	glm::mat4 matrix = glm::mat4(1.0f);

	matrix = glm::translate(matrix, position);
	matrix *= glm::mat4_cast(rotation);
	matrix = glm::scale(matrix, scale);


	//std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;

	return matrix; //check if this doesnt cause some kind of scope error
}

Entity::Entity(Transform transform)
{
	transform = transform;
}

Entity::Entity() {
	transform = Transform();
}

void Entity::SetShaderProps(Shader shader) {
	Camera* camera = ctx->camera;
	glm::mat4 view = ctx->camera->GetViewMatrix();
	shader.setMat4("view", view);

	glm::mat4 model = this->transform.ModelMatrix();
	glm::mat3 normalModel = glm::transpose(glm::inverse(model));

	shader.setMat4("model", model);
	shader.setMat3("normalModel", normalModel);
	shader.setVec3("viewPos", camera->transform.position);
	shader.setMat4("projection", camera->GetProjectionMatrix());

	shader.setVec3("material.specular", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setFloat("material.shininess", 32.0f);

	ctx->SetLightInfo(shader);
}

void Entity::Update() {};
void Entity::Render() {};

Entity::~Entity()
{
}
