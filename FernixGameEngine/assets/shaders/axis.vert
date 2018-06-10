#version 440 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat3 normalModel;

out vec3 localPos;

void main()
{
    localPos = aPos;
    gl_Position = projection * view * mat4(normalModel) * vec4(aPos * 100, 1.0);
}