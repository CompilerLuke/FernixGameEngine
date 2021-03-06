#include "Editor.h"
#include "Shader.h"
#include "Camera.h"
#include "Entity.h"
#include <math.h>
#include "imgui/imgui.h"

#include "imgizmo/imguizmo.h"
#include "metalib/meta.h"

DEFTYPE(Editor, Entity);

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
	if (input.keyPressed(GLFW_KEY_T)) { //cant use S because it is used for movement
		movingSelected = false;
		scalingSelected = true; 
		rotatingSelected = false;
	}
	if (input.keyPressed(GLFW_KEY_R)) {
		movingSelected = false;
		scalingSelected = false;
		rotatingSelected = true;
	}
	if (input.keyPressed(GLFW_KEY_ESCAPE)) {
		movingSelected = false;
		scalingSelected = false;
		rotatingSelected = false;
	}
}

void renderStruct(StructType* type, void* data);

void renderProperty(const Member& member, void* structData) {
	void* fieldP = (char*)structData + member.offset;

	if (member.name == "children") { return;  }

	switch (member.type->type) {
	case TypeEnum::Struct: {
		StructType* structT = (StructType*)member.type;
		if (structT->name == "glm_vec3") {
			ImGui::InputFloat3(member.name.c_str(), glm::value_ptr(*(glm::vec3*)fieldP)); //for some reason cannot edit scale, very wierd indeed
			return;
		}
		if (structT->name == "glm_quat") {
			glm::quat q = *(glm::quat*)fieldP;
			glm::vec3 eulerAngles = glm::eulerAngles(q);
			eulerAngles.x = glm::degrees(eulerAngles.x);
			eulerAngles.y = glm::degrees(eulerAngles.y);
			eulerAngles.z = glm::degrees(eulerAngles.z);

			ImGui::InputFloat3(member.name.c_str(), glm::value_ptr(eulerAngles));

			*(glm::quat*)fieldP = glm::quat(glm::vec3(glm::radians(eulerAngles.x), glm::radians(eulerAngles.y), glm::radians(eulerAngles.z))); //might work not sure though
			
			return;
		}
		ImGui::Text(member.name.c_str());
		ImGui::NewLine();
		renderStruct((StructType*)member.type, fieldP);
		break;
	}
	case TypeEnum::Int: 
		ImGui::InputInt(member.name.c_str(), (int*)fieldP);


		break;
	case TypeEnum::Float:
		ImGui::InputFloat(member.name.c_str(), (float*)fieldP);
		break;
	case TypeEnum::Pointer: {
		renderProperty(Member(member.name, 0, ((PointerType*)member.type)->type), *(void**)fieldP);
		break;
	}
	case TypeEnum::Array: {
		//mGui::Indent();
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
	ImGui::NewLine();

	Type* type = toRealType(selected->type);

	if (selected->type->type != TypeEnum::Struct) {
		ImGui::Text("Expecting type to be of type Struct or Class");
		return;
	}

	renderStruct((StructType*)type, selected);
}

inline bool inBetween(float num, float upper, float lower) {
	return num <= upper && num >= lower;
}

void Editor::Render() {					
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	if (!ctx->inGame) {
		if (selected) {			
			glm::vec3 scaleBefore = selected->scale;

			renderProperties(selected);

			Shader* normalShader = selected->shader;

			glm::vec3 color(1, 0.7, 0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			selected->shader = this->shader;
			selected->shader->use();
			selected->shader->setVec3("color", color);

			selected->scale = glm::vec3(scaleBefore.x * 1.001, scaleBefore.y * 1.001, scaleBefore.z * 1.001);
			selected->Render();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			selected->scale = scaleBefore;

			
			selected->shader = normalShader;

			if (movingSelected || scalingSelected || rotatingSelected) {
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
				else if (rotatingSelected) {
					mode = ImGuizmo::ROTATE;
				}

				glm::vec3 eulerAngles = glm::eulerAngles(selected->rotation);
				eulerAngles.x = glm::degrees(eulerAngles.x); //depends on if guizmo works with radians or not
				eulerAngles.y = glm::degrees(eulerAngles.y);
				eulerAngles.z = glm::degrees(eulerAngles.z);
				
				ImGuizmo::Manipulate(
					viewMatrixF, 
					projectionMatrixF, 
					mode, 
					ImGuizmo::WORLD, 
					modelMatrixF, 
					deltaMatrixF
				);

				glm::vec3 newScale = selected->scale;

				ImGuizmo::DecomposeMatrixToComponents(modelMatrixF, glm::value_ptr(selected->position), glm::value_ptr(eulerAngles), glm::value_ptr(newScale));
				selected->rotation = glm::quat(glm::vec3(glm::radians(eulerAngles.x), glm::radians(eulerAngles.y), glm::radians(eulerAngles.z)));

				if (!inBetween(newScale.x, 0.001, -0.001) && !inBetween(newScale.y, 0.001, -0.001) && !inBetween(newScale.z, 0.001, -0.001)) {
					selected->scale = newScale;
				}

			}

		}
		else {
			ImGuiIO& io = ImGui::GetIO();

			ImGuizmo::BeginFrame();
			ImGuizmo::Enable(false);
		}
	}
}
