#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 lighting;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalModel;

struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	vec3 direction;
};


uniform vec3 viewPos;
uniform Material material;

uniform vec2 transformUVs;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y) * transformUVs;
	Normal = normalModel * aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
}