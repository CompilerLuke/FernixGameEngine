#include "Shader.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "Light.h"

void DirLight::setUniforms(unsigned int id, Shader& shader) {
	shader.setVec3("dirLightr.ambient", ambient);
	shader.setVec3("dirLight.specular", specular);
	shader.setVec3("dirLight.diffuse", diffuse);

	shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f)); //glm::eulerAngles(transform.rotation));
}

void PointLight::setUniforms(unsigned int id, Shader& shader) {
	std::string prefix = "pointLights[" + id;
	shader.setVec3(prefix + "].ambient", ambient);
	shader.setVec3(prefix + "].position", transform.position);
	shader.setVec3(prefix + "].direction", glm::eulerAngles(transform.rotation));

	shader.setVec3(prefix + "].specular", specular);
	shader.setVec3(prefix + "].diffuse", diffuse);

	shader.setFloat(prefix + "].constant", 1.0f);
	shader.setFloat(prefix + "].linear", 0.09f);
	shader.setFloat(prefix + "].  quadratic", 0.032f);
};

PointLight::PointLight(float constant, float linear, float quadratic) : constant(constant), linear(linear), quadratic(quadratic) {}