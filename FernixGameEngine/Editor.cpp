#include "Editor.h"
#include "Shader.h"

class Input;
extern Input input;

Editor::Editor() : Entity() {
	this->shader = new Shader("pbr.vert", "selected.frag");
}

void Editor::Update() {
	if (input.keyPressed(GLFW_KEY_P)) {
		this->ctx->SetInGame(!this->ctx->inGame);
		std::cout << "p was pressed" << std::endl;
		input.captureMouse(false);
	}
	if (input.keyPressed(GLFW_KEY_G)) {
		movingSelected = true;
	}
	if (!this->ctx->inGame && movingSelected) {
		if (input.keyPressed(GLFW_KEY_X)) {
			axis = Axis::X;
			std::cout << "axis x" << std::endl;
		}
		else if (input.keyPressed(GLFW_KEY_Y)) {
			axis = Axis::Y;
		}
		else if (input.keyPressed(GLFW_KEY_Z)) {
			axis = Axis::Z;
		}
	}
}
float line_vertex[] =
{
	1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0
};

Line::Line() : shader(Shader("axis.vert", "selected.frag")) {
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &line_vertex[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
}

void Line::Render() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void Editor::Render() {
	if (!ctx->inGame) {
		if (selected) {
			Shader* normalShader = selected->shader;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			selected->shader = this->shader;
			selected->shader->use();
			selected->shader->setVec3("color", glm::vec3(0.5, 0.5, 0));
			selected->Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		

			if (movingSelected && axis != None) {
				if (axis != None) {					
					line.shader.use();
					this->SetShaderProps(line.shader);
					switch (axis) {
					case X:
						selected->shader->setVec3("color", glm::vec3(0, 1, 0));
						break;
					case Y:
						selected->shader->setVec3("color", glm::vec3(1, 0, 0));
						break;
					case Z:
						selected->shader->setVec3("color", glm::vec3(0, 0, 1));
						break;
					}

					line.Render();
				}
			}


			
			selected->shader = normalShader;
		}
	}
}
