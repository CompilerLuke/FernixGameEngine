#include "Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 Transform::modelMatrix() {
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::scale(matrix, scale);
	matrix = glm::rotate(matrix, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
	matrix = glm::translate(matrix, position);
	return matrix; //check if this doesnt cause some kind of scope error
}



Entity::Entity(Transform transform)
{
	transform = transform;
}

Entity::Entity() {
	transform = Transform();
}

void Entity::update(float deltaTime) {};
void Entity::render() {};

Entity::~Entity()
{
}
