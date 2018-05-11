#pragma once

#include "Spatial.h"
#include "Entity.h"
#include "Model.h"

class Shader;

class Spatial : public Entity
{
public:
	Spatial(Model* model, Shader* shader);
	~Spatial();

	virtual void Render() override;

	Shader* shader;
	Model* model;
};

