#include "Editor.h"
#include "Shader.h"
#include "Camera.h"
#include "Entity.h"
#include <math.h>
#include "imgui/imgui.h"

#include "imgizmo\imguizmo.h"

class Input;
extern Input input;

glm::vec2 offset;

Editor::Editor() : Entity() {
	this->shader = new Shader("assets/shaders/pbr.vert", "assets/shaders/selected.frag");
}

void Editor::Update() {
	if (input.keyPressed(GLFW_KEY_P)) {
		this->ctx->SetInGame(!this->ctx->inGame);
		std::cout << "p was pressed" << std::endl;
		input.captureMouse(ctx->inGame);
	}
	if (input.keyPressed(GLFW_KEY_G)) {
		movingSelected = true;
		std::cout << "moving" << std::endl;
	}
	if (input.keyPressed(GLFW_KEY_ESCAPE)) {
		movingSelected = false;
	}
}

float line_vertex[] =
{
	1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0
};

Line::Line() : shader(Shader("assets/shaders/axis.vert", "assets/shaders/selected.frag")) {
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
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	if (!ctx->inGame) {
		if (selected) {
			Shader* normalShader = selected->shader;

			{
				ImGui::NewLine();


				ImGui::InputFloat3("Position", glm::value_ptr(selected->position), 3);
				ImGui::InputFloat3("Rotation", &selected->rotation.x, 3);
				ImGui::InputFloat3("Scale", glm::value_ptr(selected->scale), 3);

				ImGui::NewLine();
			}

			glm::vec3 scale = selected->scale;

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			selected->shader = this->shader;
			selected->shader->use();
			selected->shader->setVec3("color", glm::vec3(0.5, 0.5, 0));

			selected->scale = glm::vec3(scale.x * 1.001, scale.y * 1.001, scale.z * 1.001);
			selected->Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			selected->scale = scale;

			
			selected->shader = normalShader;

			if (movingSelected) {
				glm::mat4 modelMatrix = selected->ModelMatrix();
				glm::mat4 deltaMatrix;

				glm::mat4 viewMatrix = ctx->camera->GetViewMatrix();
				glm::mat4 projectionMatrix = ctx->camera->GetProjectionMatrix();

				const float* viewMatrixF = glm::value_ptr(viewMatrix);
				const float* projectionMatrixF = glm::value_ptr(projectionMatrix);
				float* modelMatrixF = glm::value_ptr(modelMatrix);
				float* deltaMatrixF = glm::value_ptr(deltaMatrix);				
				
				ImGuiIO& io = ImGui::GetIO();

				ImGuizmo::BeginFrame();
				ImGuizmo::Enable(true);
				ImGuizmo::SetOrthographic(false);

				ImGuizmo::SetRect(0, 0, ctx->SCR_WIDTH, ctx->SCR_HEIGHT);
				
				ImGuizmo::Manipulate(
					viewMatrixF, 
					projectionMatrixF, 
					ImGuizmo::TRANSLATE, 
					ImGuizmo::WORLD, 
					modelMatrixF, 
					deltaMatrixF
				);

				ImGuizmo::DecomposeMatrixToComponents(modelMatrixF, glm::value_ptr(selected->position), glm::value_ptr(selected->rotation), glm::value_ptr(selected->scale));
			}
		}
	}
}
