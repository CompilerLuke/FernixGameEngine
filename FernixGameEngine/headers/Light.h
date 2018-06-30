#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include "Shader.h"
#include "Entity.h"

struct Entity;
class Shader;

class Light : public Entity {
protected:
	glm::vec3 color = glm::vec3(1.0f);

public:
	Light(glm::vec3 color);
	virtual void setUniforms(unsigned int id, Shader shader) {};
	virtual ~Light() {};
};

class DirLight : public Light {
public:
	DirLight(glm::vec3 color = glm::vec3(1.0f));

	virtual void setUniforms(unsigned int id, Shader shader) override;
};

class PointLight : public Light {
public:
	PointLight(glm::vec3 color = glm::vec3(1.0f));

	virtual void setUniforms(unsigned int id, Shader shader) override;
};

struct StructType;
extern StructType* DirLight;
extern StructType* PointLight;