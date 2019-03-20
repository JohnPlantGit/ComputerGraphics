// Frag shader
#version 440

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;

//uniform sampler2D Kd;
uniform vec3 Ia; // ambient light colour
uniform vec3 Id; // diffuse light colour
uniform vec3 Is; // specular light colour
uniform vec3 LightDirection;

uniform vec3 Ka; // ambient material colour
uniform sampler2D Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float SpecularPower; // material specular power

uniform vec3 CameraPosition;

out vec4 FragColour;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	float lambertTerm = max(0, min(1, dot(N, -L)));

	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = relect(L, N);

	float SpecularTerm = pow(max(0, dot(R, V)), SpecularPower)

	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * texture( Kd, vTexCoord ).xyz * lambertTerm;
	vec3 specular = Is * Ks * SpecularTerm;

	//FragColour = texture( Kd, vTexCoord );
	FragColour = vec4(ambient + diffuse + specular, 1);
}