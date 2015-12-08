#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float animPosition;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex
{
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void)
{
	float f = 1 - animPosition;
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0)
								* mat4(
											 vec4(f, 0, 0, 0),
											 vec4(0, f, 0, 0),
											 vec4(0, 0, f, 0),
											 vec4(0, 0, 0, 1)
											 );
	
	OUT.texCoord = texCoord;
	OUT.colour = colour;
}