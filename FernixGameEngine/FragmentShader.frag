#version 440 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 lighting;

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
uniform Light light;  

void main()
{

	vec3 lightDir = normalize(light.direction);
	vec3 norm = normalize(Normal);

	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	//diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));  

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}