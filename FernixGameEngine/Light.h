#pragma once

#include "Entity.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

class Entity;

class Light: public Entity {
protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	Light() {};
	virtual void setUniforms(Shader shader) {};
	virtual ~Light() {};
};

class DirLight : public Light {
protected:

public:
	DirLight() {}

	virtual void setUniforms(Shader shader) {
		shader.setVec3("light.ambient", ambient);
		shader.setVec3("light.specular", specular);
		shader.setVec3("light.diffuse", diffuse);

		shader.setVec3("light.direction", glm::eulerAngles(transform.rotation));
	};
};

class PointLight : public Light {
protected:
	float constant;
	float linear;
	float quadratic;

public:
	PointLight(float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f)
		: constant(constant), linear(linear), quadratic(quadratic) {}

	virtual void setUniforms(unsigned int id, Shader shader) {
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
};