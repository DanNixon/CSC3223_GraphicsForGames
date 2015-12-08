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
	vec4 normalCol = texture(objectTextures[0], IN.texCoord);
	vec4 destroyedCol = texture(objectTextures[1], IN.texCoord);
	
	fragCol = normalCol * (1 - animPosition) + destroyedCol * animPosition;
}