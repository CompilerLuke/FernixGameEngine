#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

class Entity;
class Shader;

class Light: public Entity {
protected:
	glm::vec3 ambient = glm::vec3(0.2f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

public:
	Light() {};
	virtual void setUniforms(unsigned int id, Shader& shader) {};
	virtual ~Light() {};
};

class DirLight : public Light {
protected:

public:
	DirLight() {}

	virtual void setUniforms(unsigned int id, Shader& shader) override;
};

class PointLight : public Light {
protected:
	float constant;
	float linear;
	float quadratic;

public:
	PointLight(float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);

	virtual void setUniforms(unsigned int id, Shader& shader) override;
};