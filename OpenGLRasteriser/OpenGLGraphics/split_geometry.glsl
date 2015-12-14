#version 330 core

uniform float animPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (triangles) in;
layout (triangle_strip, max_vertices = 256) out;

in Vertex
{
	vec2 texCoord;
	vec4 colour;
} IN[];

out Vertex
{
	vec2 texCoord;
	vec4 colour;
} OUT;

vec4 triPos(float s, float t)
{
	vec4 a = gl_in[0].gl_Position;
	vec4 b = gl_in[1].gl_Position;
	vec4 c = gl_in[2].gl_Position;

	return s * a + t * b + (1 - s - t) * c;
}

void cubeVertex(mat4 mvp, vec4 pos, vec3 vPos)
{
	gl_Position = mvp * vec4(pos.xyz + vPos, 1);
	EmitVertex();
}

void cubeAtPoint(vec4 p, float s)
{
	vec3 upper = vec3(0, 0, s);
	vec3 lower = vec3(0, 0, -s);
	vec3 left = vec3(-s, 0, 0);
	vec3 right = vec3(s, 0, 0);
	vec3 front = vec3(0, s, 0);
	vec3 back = vec3(0, -s, 0);
	
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;

	// First strip (front and side faces)
	cubeVertex(mvp, p, upper + left + back);
	cubeVertex(mvp, p, lower + left + back);
	cubeVertex(mvp, p, upper + left + front);
	cubeVertex(mvp, p, lower + left + front);
	cubeVertex(mvp, p, upper + right + front);
	cubeVertex(mvp, p, lower + right + front);
	cubeVertex(mvp, p, upper + right + back);
	cubeVertex(mvp, p, lower + right + back);
	EndPrimitive();
	
	// Second strip (top, bottom and side faces)
	cubeVertex(mvp, p, upper + left + front);
	cubeVertex(mvp, p, upper + right + front);
	cubeVertex(mvp, p, upper + left + back);
	cubeVertex(mvp, p, upper + right + back);
	cubeVertex(mvp, p, lower + left + back);
	cubeVertex(mvp, p, lower + right + back);
	cubeVertex(mvp, p, lower + left + front);
	cubeVertex(mvp, p, lower + right + front);
	EndPrimitive();
}

void main()
{
	float diff = 0.2; //max(min(1 - animPosition, 0.6), 0.18);
	float size = diff - (diff / 4);

	for (float s = 0; s < 1; s += diff)
	{
		for (float t = 0; t < 1; t += diff)
		{
			if (s + t > 1)
				continue;

			cubeAtPoint(triPos(s, t), size);
		}
	}
}