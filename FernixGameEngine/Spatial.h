#pragma once

#include "Spatial.h"
#include "Entity.h"
#include "Model.h"
#include "Render.h"

class Shader;
class Render;

class Spatial : public Entity
{
public:
	Spatial(Model* model, Shader* shader);
	~Spatial();

	virtual void Render() override;

	Shader* shader;
	Model* model;
};

