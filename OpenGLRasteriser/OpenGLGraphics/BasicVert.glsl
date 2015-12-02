#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform vec4 colours[5];
uniform int selectedColour;

in vec3 position;
in vec2 texCoord;
in vec4 colour;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void)	{
	gl_Position		= (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	
	OUT.texCoord	= texCoord;
	if (selectedColour == 0)
		OUT.colour = colour;
	else
		OUT.colour = colours[selectedColour - 1];
}