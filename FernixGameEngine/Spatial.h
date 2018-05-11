#pragma once

#include "Spatial.h"
#include "Entity.h"
#include "Model.h"

class Shader;

class Spatial : Entity
{
public:
	Spatial(Model* model, Shader* shader);

	virtual void Render() override;

private:	
	Shader* shader;
	Model* model;
	Spatial();
	~Spatial();
};

