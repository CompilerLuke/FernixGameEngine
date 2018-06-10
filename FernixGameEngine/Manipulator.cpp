#include "Manipulator.h"
#include "Input.h"
#include "Camera.h"

#include <IGizmo.h>

extern Input input;
bool initGizmo = false;
IGizmo *gizmo = NULL;
IGizmo *gizmoMove, *gizmoRotate, *gizmoScale;

void loadGizmo(Render * ctx, float * objectMatrix) {
	if (initGizmo) {
		gizmo->SetEditMatrix(objectMatrix);
		gizmo->SetScreenDimension(ctx->SCR_WIDTH, ctx->SCR_HEIGHT);
		return;
	}

	CGizmoTransformRenderInit();

	initGizmo = true; //dont want infinite loop if it doesnt load

	gizmoMove = CreateMoveGizmo();
	gizmoRotate = CreateRotateGizmo();
	gizmoScale = CreateScaleGizmo();

	gizmo = gizmoMove;

	gizmo->SetEditMatrix(objectMatrix);
	gizmo->SetScreenDimension(ctx->SCR_WIDTH, ctx->SCR_HEIGHT);
	gizmoMove->SetDisplayScale(2.f);
	gizmoRotate->SetDisplayScale(2.f);
	gizmoScale->SetDisplayScale(2.f);
}

void Manipulator::operator=(const Manipulator& e) {
	this->shader = e.shader;
	this->model = e.model;
	this->modelMatrix = e.modelMatrix;
	this->myTransform = e.myTransform;
	this->ctx = e.ctx;
}

Manipulator::Manipulator(Entity* transform) :
	myTransform(transform), modelMatrix(glm::value_ptr(transform->ModelMatrix())) {
}

void Manipulator::Init() {
	loadGizmo(this->ctx, (float*)modelMatrix); //ignore const not sure if this is so smart
}

void Manipulator::Render() {
	gizmo->SetCameraMatrix(modelMatrix, glm::value_ptr(ctx->camera->GetProjectionMatrix()));

	if (input.rightClicking()) {
		gizmo->OnMouseMove(input.mouse_position.x, input.mouse_position.y);
	}
	gizmo->Draw();
}

void Manipulator::Update() {
	myTransform->position = glm::vec3(modelMatrix[3], modelMatrix[7], modelMatrix[11]);
}