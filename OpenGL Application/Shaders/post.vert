// Vert shader
#version 440

layout( location = 0 ) in vec4 Position;

out vec2 vTexCoord;

void main() 
{
	vTexCoord = Position.xy * 0.5f + 0.5f;
	gl_Position = vec4(Position.xy, 0, 1);
}