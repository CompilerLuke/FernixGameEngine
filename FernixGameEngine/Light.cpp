#include "Shader.h"
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "Light.h"

#include "metalib/meta.h"

DEFTYPE(PointLight, Entity,
	MEMBER(PointLight, color, glm_vec3)
);

DEFTYPE(DirLight, Entity,
	MEMBER(DirLight, color, glm_vec3)
);



void DirLight::setUniforms(unsigned int id, Shader shader) {
	shader.setVec3("dirLightr.ambient", color);
	shader.setVec3("dirLight.direction", glm::eulerAngles(rotation));
}

void PointLight::setUniforms(unsigned int id, Shader shader) {
	std::string prefix = "pointLights[" + id;
	shader.setVec3(prefix + "].position", position);
	shader.setVec3(prefix + "].color", color);

};

PointLight::PointLight(glm::vec3 color) : Light(color) {}
DirLight::DirLight(glm::vec3 color) : Light(color) {}
Light::Light(glm::vec3 color) : Entity() {
	color = color;
}

