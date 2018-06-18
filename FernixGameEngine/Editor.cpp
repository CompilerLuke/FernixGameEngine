#include "Editor.h"
#include "Shader.h"
#include "Camera.h"
#include "Entity.h"
#include <math.h>
#include "imgui/imgui.h"

#include "imgizmo/imguizmo.h"
#include "metalib/meta.h"

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
		scalingSelected = false;
		rotatingSelected = false;
	}
	if (input.keyPressed(GLFW_KEY_S)) {
		movingSelected = false;
		scalingSelected = true;
		rotatingSelected = false;
	}
	if (input.keyPressed(GLFW_KEY_ESCAPE)) {
		movingSelected = false;
	}
}

void renderStruct(StructType* type, void* data);

unsigned int uniqueName = 0;

void renderProperty(const Member& member, void* structData) {
	void* fieldP = (char*)structData + member.offset;

	switch (member.type->type) {
	case TypeEnum::Struct: {
		ImGui::Text(member.name.c_str());
		ImGui::NewLine();
		//ImGui::Indent();
		renderStruct((StructType*)member.type, fieldP);
		//ImGui::Unindent();
		break;
	}
	case TypeEnum::Int: 
		ImGui::InputInt(member.name.c_str(), (int*)fieldP);


		break;
	case TypeEnum::Float:
		uniqueName++;
		ImGui::InputFloat(std::to_string(uniqueName).c_str(), (float*)fieldP);
		break;
	case TypeEnum::Pointer: {
		renderProperty(Member(member.name, 0, ((PointerType*)member.type)->type), *(void**)fieldP);
		break;
	}
	case TypeEnum::Array: {
		//ImGui::Indent();
		std::vector<char> array = *(std::vector<char>*)fieldP;
		ArrayType* arrayType = (ArrayType*)member.type;


		unsigned int len = array.size() / sizeOf(arrayType->type);


		for (unsigned int i = 0; i < len; i++) { //divide by sizeof arrayType
			Member member(std::to_string(i), i * sizeOf(arrayType->type), arrayType->type);
			renderProperty(member, array.data());
		}
		//ImGui::Unindent();
		break;
	}
	case TypeEnum::String: {

		std::string s = *(std::string*)fieldP;
		ImGui::Text((member.name + " : " + s).c_str());
		break;
	}
	default: {
		std::string s = member.name + " : unknown type";
		ImGui::Text(s.c_str());
		ImGui::NewLine();
	}
	}
};

void renderStruct(StructType* type, void* data) {
	if (type->isTyped) {
		type = (StructType*)toRealType(((Typed*)data)->type); //for polymorphic objects
	}
	ImGui::Text(type->name.c_str());
	ImGui::NewLine();
	//ImGui::Indent();
	for (unsigned int i = 0; i < type->members.size(); i++) {
		renderProperty(type->members[i], data);
	}
	//ImGui::Unindent();
}

void renderProperties(Entity* selected) {
	uniqueName = 0;
	ImGui::NewLine();

	Type* type = toRealType(selected->type);

	if (selected->type->type != TypeEnum::Struct) {
		ImGui::Text("Expecting type to be of type Struct or Class");
		return;
	}

	renderStruct((StructType*)type, selected);
}

void Editor::Render() {					
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	if (!ctx->inGame) {
		if (selected) {
			renderProperties(selected);

			Shader* normalShader = selected->shader;

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

			if (movingSelected || scalingSelected) {
				if (selected->position.x == 0) {
					selected->position.x = 0.0001f;
				}
				if (selected->position.y == 0) {
					selected->position.y = 0.0001f;
				}
				if (selected->position.z == 0) {
					selected->position.z = 0.0001f;
				}
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

				ImGuizmo::OPERATION mode;
				if (movingSelected) {
					mode = ImGuizmo::TRANSLATE;
				}
				else if (scalingSelected) {
					mode = ImGuizmo::SCALE;
				}
				
				ImGuizmo::Manipulate(
					viewMatrixF, 
					projectionMatrixF, 
					mode, 
					ImGuizmo::WORLD, 
					modelMatrixF, 
					deltaMatrixF
				);

				ImGuizmo::DecomposeMatrixToComponents(modelMatrixF, glm::value_ptr(selected->position), glm::value_ptr(selected->rotation), glm::value_ptr(selected->scale));
			}
		}
	}
}
