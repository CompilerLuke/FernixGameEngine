#include "Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include <iostream>

glm::mat4 Transform::modelMatrix() {
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::scale(matrix, scale);
	matrix *= glm::mat4_cast(rotation);
	matrix = glm::translate(matrix, position);

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

void Entity::SetShaderProps(Shader& shader, Camera& camera) {
	
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("view", view);

	glm::mat4 model = this->transform.modelMatrix();
	glm::mat3 normalModel = glm::transpose(glm::inverse(model));

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection(0.7f, 0.7f, 0.0f);

	shader.setMat4("model", model);
	shader.setMat3("normalModel", normalModel);
	shader.setVec3("viewPos", camera.transform.position);
	shader.setMat4("projection", camera.GetProjectionMatrix());

	shader.setVec3("material.specular", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setFloat("material.shininess", 32.0f);

	shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // darken the light a bit to fit the scene
	shader.setVec3("light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3("light.direction", lightDirection);
}

void Entity::update(float deltaTime) {};
void Entity::render(Camera& camera) {};

Entity::~Entity()
{
}
