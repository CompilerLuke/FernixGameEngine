#pragma once

#include "Entity.h"
#include "Input.h"



struct Editor : Entity {
	Entity* selected;
	bool movingSelected = false;
	bool scalingSelected = false;
	bool rotatingSelected = false;

	void Update() override;
	void Render() override;

	Editor();
};