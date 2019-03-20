// Vert shader
#version 440

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoord;

uniform mat4 ProjectionViewModel;
unifrom mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform float time;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vPosition

void main() 
{
	vPosition = ModelMatrix * Position;
	vTexCoord = TexCoord;
	vNormal = NormalMatrix * Normal.xyz;
	gl_Position = ProjectionViewModel * Position;// * (Position + vec4(sin(Position.y + time) * 0.1, 0, cos(Position.y + time) * 0.1, 0));
}