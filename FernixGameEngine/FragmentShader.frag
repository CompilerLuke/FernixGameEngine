#version 440 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightDirection;

void main()
{
	vec3 color = vec3(texture(ourTexture, TexCoord));
	vec3 lightDir = normalize(lightDirection);
	vec3 norm = normalize(Normal);

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (diff + ambient) * color;

    FragColor = vec4(result, 1.0);
}