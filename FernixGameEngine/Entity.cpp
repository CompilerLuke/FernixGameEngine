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

DEFTYPE_NAMESPACE(glm, quat, NULL,
	MEMBER(glm::quat, w, float),
	MEMBER(glm::quat, x, float),
	MEMBER(glm::quat, y, float),
	MEMBER(glm::quat, z, float),
)

DEFTYPE(Entity, Typed,
	MEMBER(Entity, position, glm_vec3),
	MEMBER(Entity, rotation, glm_quat),
	MEMBER(Entity, scale, glm_vec3),
	//MEMBER_NT(Entity, model, POINTER(Model)),
	MEMBER_NT(Entity, children, ARRAY_NT(POINTER_NT(FORWARD(Entity)))),
	MEMBER_NT(Entity, ctx, POINTER(Render))
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

Entity::Entity(Model* model, Shader* shader) : Typed(FORWARD(Entity))
{
	model = model;
	shader = shader;
}

Entity::Entity() : Typed(FORWARD(Entity)) {
	model = NULL;
	shader = NULL;
	parent = NULL;
}

unsigned int ID_NUM = 0;
void Entity::NewID() {
	this->ID = ++ID_NUM;
}

void Entity::SetParent(Entity* newParent) {
	if (parent) {
		for (unsigned int i = 0; i < parent->children.size(); i++) {
			if (parent->children[i] == this) {
				parent->children[i] = parent->children[parent->children.size() - 1];
				parent->children.resize(parent->children.size() - 1);
				break;
			}
		}
	}

	if (newParent->ctx) {
		ctx = newParent->ctx;
	}
	else if (ctx && !newParent->ctx) {
		parent->ctx = ctx;
	}
	
	parent = newParent;
	newParent->children.push_back(this);
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
