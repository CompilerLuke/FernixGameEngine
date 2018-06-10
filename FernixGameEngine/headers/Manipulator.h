#pragma once

#include <GLFW/glfw3.h>
#include "Entity.h"

struct Manipulator : Entity {
	const float* modelMatrix; 
	Entity* myTransform;

	Manipulator(Entity* transform);
	
	void operator=(const Manipulator& e);

	void Init();
	void Render() override;
	void Update() override;
};
