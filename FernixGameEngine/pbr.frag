#version 440 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

uniform samplerCube irradianceMap;

struct Material {
	sampler2D diffuse;
	sampler2D metallic;
	sampler2D roughness;
	sampler2D ao;
	sampler2D normal;
};

struct DirLight {
	vec3 direction;
    vec3 color;
};

struct PointLight {
	vec3 position;  
    vec3 color;
};

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;  
#define MAX_NR_POINT_LIGHTS 4  

uniform int NR_POINT_LIGHTS;
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];

const float PI = 3.14159265359;

vec3 albedo;
vec3 normal;
float metallic;
float roughness;
float ao;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 CalcDirLight(DirLight light, vec3 N, vec3 V) {
	vec3 L = normalize(-light.direction);
	vec3 H = normalize(V + L);

    vec3 radiance     = light.color; 

	vec3 F0 = vec3(0.04); 
	F0      = mix(F0, albedo, metallic);
	vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

	float NDF = DistributionGGX(N, H, roughness);       
	float G   = GeometrySmith(N, V, L, roughness);    

	//cook torrance BRDF
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular     = numerator / max(denominator, 0.001);  

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
  
	kD *= 1.0 - metallic;	

	const float PI = 3.14159265359;
  
    float NdotL = max(dot(N, L), 0.0);        
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 WorldPos, vec3 V)
{
    vec3 L = normalize(light.position - WorldPos);
	vec3 H = normalize(V + L);

	float distance    = length(light.position - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = light.color * attenuation; 

	vec3 F0 = vec3(0.04); 
	F0      = mix(F0, albedo, metallic);
	vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

	float NDF = DistributionGGX(N, H, roughness);       
	float G   = GeometrySmith(N, V, L, roughness);    

	//cook torrance BRDF
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular     = numerator / max(denominator, 0.001);  

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
  
	kD *= 1.0 - metallic;	

	const float PI = 3.14159265359;
  
    float NdotL = max(dot(N, L), 0.0);        
    return (kD * albedo / PI + specular) * radiance * NdotL;
} 

void main()
{
    // properties
    vec3 norm = texture(material.normal, TexCoords).rgb;
	norm = normalize(norm * 2.0 - 1.0);   
	norm = normalize(TBN * norm);
	
    vec3 viewDir = normalize(viewPos - FragPos);

	albedo     = pow(texture(material.diffuse, TexCoords).rgb, vec3(2.2));

    normal     = Normal; //getNormalFromNormalMap();
    metallic  = texture(material.metallic, TexCoords).r;
    roughness = texture(material.roughness, TexCoords).r;
    ao        = texture(material.ao, TexCoords).r;

	vec3 Lo = vec3(0.0);
	vec3 WorldPos = FragPos;
	vec3 V = viewDir;
	vec3 N = norm;

	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // phase 1: Directional lighting
    Lo += CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        Lo += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  
	}

    //Lo += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

	vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness); 
	vec3 kD = 1.0 - kS;
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse    = irradiance * albedo;
	vec3 ambient    = (kD * diffuse) * ao; 

    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(ambient, 1.0);
}