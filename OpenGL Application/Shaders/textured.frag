// Vert shader
#version 440

in vec2 vTexCoord;

uniform sampler2D Kd; // diffuse material colour

out vec4 FragColour;

void main() 
{
	FragColour = vec4(texture(Kd, vTexCoord).xyz, 1);	
}