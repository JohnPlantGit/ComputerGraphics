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

uniform Light lights[LIGHT_COUNT];
uniform sampler2D Kd; // diffuse material colour

out vec4 FragColour;

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec2 texCoord);

void main() 
{
	vec3 result;
	
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		result += CalculateLight(lights[i], vNormal, vPosition, vTexCoord);
	}

	FragColour = vec4(result, 1);
	
}

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec2 texCoord)
{
	float intensity = dot(light.direction, normalize(-normal));

	if (intensity > 0.95)
		return texture( Kd, texCoord ).xyz * 1;
	else if (intensity > 0.75)
		return texture( Kd, texCoord ).xyz * 0.75;
	else if (intensity > 0.5)
		return texture( Kd, texCoord ).xyz * 0.5;
	else
		return texture( Kd, texCoord ).xyz * 0.25;
}