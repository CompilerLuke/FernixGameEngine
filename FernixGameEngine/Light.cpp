#include "Shader.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "Light.h"

void DirLight::setUniforms(unsigned int id, Shader shader) {
	shader.setVec3("dirLightr.ambient", color);
	shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f)); //glm::eulerAngles(transform.rotation));
}

void PointLight::setUniforms(unsigned int id, Shader shader) {
	std::string prefix = "pointLights[" + id;
	shader.setVec3(prefix + "].position", transform.position);
	shader.setVec3(prefix + "].color", color);

};

PointLight::PointLight(glm::vec3 color) : Light(color) {}
DirLight::DirLight(glm::vec3 color) : Light(color) {}
Light::Light(glm::vec3 color) {
	color = color;
}