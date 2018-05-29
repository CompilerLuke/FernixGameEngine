#version 440 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

uniform vec3 color;

void main() {
	FragColor = vec4(color, 1);
}