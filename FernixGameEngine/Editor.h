#pragma once

#include "Entity.h"
#include "Input.h"

enum Axis {
	X,
	Y,
	Z,
	None
};

struct Line {
	Shader shader;
	float vertices[28];
	unsigned int VAO;
	unsigned int VBO;

	Line();
	void Render();
};

struct Editor : Entity {
	Entity* selected;
	bool movingSelected = false;
	Axis axis = None;
	Line line;

	void Update() override;
	void Render() override;

	Editor();
};