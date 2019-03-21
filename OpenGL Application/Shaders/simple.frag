// Frag shader
#version 440

struct Light
{
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
};
#define LIGHT_COUNT 4

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;

uniform vec3 Ia; // ambient light colour
//uniform vec3 Id; // diffuse light colour
//uniform vec3 Is; // specular light colour
uniform vec3 LightDirection;
uniform Light lights[LIGHT_COUNT];

uniform vec3 Ka; // ambient material colour
uniform sampler2D Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float specularPower; // material specular power
uniform sampler2D specularTexture; // material specular texture
uniform float Roughness; // material roughness
uniform float ReflectionCoefficient;
uniform sampler2D normalTexture;

uniform vec3 CameraPosition;

out vec4 FragColour;

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec2 texCoord);

void main()
{
	vec3 result;

	vec3 normal = normalize(vNormal + normalize(texture(normalTexture, vTexCoord) * 2.0f - 1.0f).rgb);
	
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		result += CalculateLight(lights[i], normal, vPosition, vTexCoord);
	}

	FragColour = vec4(result, 1);
}

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos, vec2 texCoord)
{
	vec3 N = normalize(normal);
	vec3 L = normalize(-light.direction);
	vec3 E = normalize(CameraPosition - fragPos);

	// Oren-Nayar Diffuse Reflectance
	float NdL = max(0.0f, dot(N, L));
	float NdE = max(0.0f, dot(N, E));

	float R2 = Roughness * Roughness;

	// Oren-Nayar Diffuse Term
	float A = 1.0f - 0.5f * R2 / (R2 + 0.33f);
	float B = 0.45 * R2 / (R2 + 0.09f);

	// CX = max(0, cos(L, E))
	vec3 lightProjected = normalize(L - N * NdL);
	vec3 viewProjected = normalize(E - N * NdE);
	float CX = max(0.0f, dot(lightProjected, viewProjected));

	// DX = sin(alpha) * tan(beta)
	float alpha = sin(max(acos(NdE), acos(NdL)));
	float beta = tan(min(acos(NdE), acos(NdL)));
	float DX = alpha * beta;

	// replaces the lambertTerm
	float orenNayar = NdL * (A + B * CX * DX);
	//float lambertTerm = max(0, min(1, dot(N, -L)));

	// Cook-Torrance Specular Reflectance
	vec3 H = normalize(L + E);

	float NdH = max(0.0f, dot(N, H));
	float NdH2 = NdH * NdH;

	float e = 2.71828182845904523536028747135f;
	float pi = 3.1415926535897932384626433832f;

	// Beckman's Distribution Function D
	float exponent = -(1 - NdH2) / (NdH2 * R2);
	float D = pow(e, exponent) / (R2 * NdH2 * NdH2);

	// Fresnel Term F
	float F = ReflectionCoefficient + (1 - ReflectionCoefficient) * pow(1 - NdE, 5);

	// Geometric Attenuation Factor G
	float X = 2.0f * NdH / dot(E, H);
	float G = min (1, min(X * NdL, X * NdE));

	float cookTorrance = max((D * G * F) / (NdE * pi), 0.0f);

	vec3 V = normalize(CameraPosition - fragPos.xyz);
	vec3 R = reflect(-L, N);

	float SpecularTerm = pow(max(0, dot(R, V)), specularPower);

	vec3 ambient = Ia * Ka;
	vec3 diffuse = light.diffuse * texture( Kd, texCoord ).xyz * orenNayar;
	vec3 specular = light.specular * texture( specularTexture, texCoord ).xyz * cookTorrance;

	return (ambient + diffuse + specular);
}