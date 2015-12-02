#version 330 core

uniform float time;
uniform sampler2D textures[5];

in Vertex
{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 fragCol;

void main(void)
{
	vec4 smileyColour = texture (textures[0], IN.texCoord);
	
	if( smileyColour.b > 0.1)
	{
		vec2 tempTex = IN.texCoord;
		tempTex.y += time;
		fragCol = texture(textures[1], tempTex);
	}
	else
	{
		fragCol = smileyColour;
	}
}