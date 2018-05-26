#include "Editor.h"
#include <GLFW\glfw3.h>

class Input;

extern Input input;

void Editor::Update() {
	if (input.keyPressed(GLFW_KEY_P)) {
		this->ctx->SetInGame(!this->ctx->inGame);
		std::cout << "p was pressed" << std::endl;
	}
}
