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

#include "metalib/meta.h"

class Render;
class Camera;

DEFTYPE_NAMESPACE(glm, vec3, NULL,
	MEMBER(glm::vec3, x, float),
	MEMBER(glm::vec3, y, float),
	MEMBER(glm::vec3, z, float),
);

DEFTYPE(Entity, Typed,
	MEMBER(Entity, position, glm_vec3),
	MEMBER(Entity, scale, glm_vec3),
	//MEMBER_NT(Entity, model, POINTER(Model))
);

glm::mat4 Entity::ModelMatrix() {
	glm::mat4 matrix;

	if (parent) {
		matrix = parent->ModelMatrix();
	}
	else {
		matrix = glm::mat4(1);
	}

	matrix = glm::translate(matrix, position);
	matrix *= glm::mat4_cast(rotation);
	matrix = glm::scale(matrix, scale);


	//std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;

	return matrix; //check if this doesnt cause some kind of scope error
}

Entity::Entity(Model* model, Shader* shader) : Typed(EntityType)
{
	model = model;
	shader = shader;
}

Entity::Entity() : Typed(EntityType) {
	model = NULL;
	shader = NULL;
	parent = NULL;
}

void Entity::SetShaderProps(Shader shader) {
	Camera* camera = ctx->camera;
	Skybox* skybox = ctx->skybox;

	glm::mat4 view = ctx->camera->GetViewMatrix();
	shader.setMat4("view", view);

	glm::mat4 model = this->ModelMatrix();
	glm::mat3 normalModel = glm::transpose(glm::inverse(model));

	shader.setMat4("model", model);
	shader.setMat3("normalModel", normalModel);
	shader.setVec3("viewPos", camera->position);
	shader.setMat4("projection", camera->GetProjectionMatrix());

	ctx->SetLightInfo(shader);
}

void Entity::Update() {};
void Entity::OnEnterGame() {};
void Entity::OnEnterEditor() {};

void Entity::Render() {
	if (shader && model) {
		shader->use();
		this->SetShaderProps(*shader);
		model->Render(*shader, ctx->skybox);
	}
};

bool Entity::RunningGame() {
	return this->ctx->inGame;
}

Entity::~Entity()
{
}
