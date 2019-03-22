// Vert shader
#version 440

struct Light
{
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};
#define LIGHT_COUNT 4

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vPosition;

uniform vec3 Ia;
uniform Light lights[LIGHT_COUNT];

uniform sampler2D Ka;
uniform sampler2D Kd; // diffuse material colour
uniform sampler2D Ks;
uniform float specularPower;

uniform vec4 outlineColour;
uniform vec3 cameraPosition;

out vec4 FragColour;

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec2 texCoord);

void main() 
{
	vec3 result;
	
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		result += CalculateLight(lights[i], vNormal, vPosition, vTexCoord);
	}

	float facingPercentage = dot(normalize(cameraPosition - vPosition), vNormal);
	FragColour = (vec4(result, 1) * facingPercentage) + outlineColour * (1 - facingPercentage);
}

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec2 texCoord)
{
	vec3 N = normalize(normal);
	vec3 L = normalize(light.direction);

	float lambertTerm = max(0, min(1, dot(N, -L)));

	vec3 V = normalize(cameraPosition - fragPos.xyz);
	vec3 R = reflect(L, N);

	float specularTerm = pow(max(0, dot(R, V)), specularPower);

	vec3 ambient = Ia * texture(Ka, texCoord).xyz;
	vec3 diffuse = light.diffuse * texture(Kd, texCoord).xyz * lambertTerm;
	vec3 specular = light.specular * texture(Ks, texCoord).xyz * specularTerm;

	return vec3(ambient + diffuse);
}