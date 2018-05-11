#include "Spatial.h"
#include "Shader.h"

Spatial::Spatial(Model* model, Shader* shader) : Entity() {
	model = model;
	shader = shader;
}

void Spatial::Render() {
	this->SetShaderProps(*shader);
	model->Render(*shader);
}

Spatial::~Spatial() {

}

