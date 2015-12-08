#version 330 core

uniform float animPosition;
uniform sampler2D objectTextures[2];

in Vertex
{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragCol;

void main(void)
{
	vec4 col = texture(objectTextures[0], IN.texCoord);
	col.a = 1 - animPosition;
	fragCol = col;
}