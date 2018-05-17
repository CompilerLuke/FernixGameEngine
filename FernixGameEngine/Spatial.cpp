#include "Spatial.h"
#include "Shader.h"

Spatial::Spatial(Model* model, Shader* shader) : Entity() {
	std::cout << "Initializing spatial" << std::endl;
	std::cout << shader << std::endl;
	model = model;
	shader = shader;
}

void Spatial::Render() {
	shader->use();
	this->SetShaderProps(*shader);
	model->Render(*shader, ctx->skybox);
}

Spatial::~Spatial() {

}

