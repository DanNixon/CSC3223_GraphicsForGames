#version 330 core

uniform float animPosition;

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

void cubeVertex(vec4 pos)
{
	gl_Position = pos;
	EmitVertex();
}

void cubeAtPoint(vec4 p, float s)
{
	vec4 pos;
	
	pos = p;
	pos.x += s;
	pos.y += s;
	pos.z += s;
	cubeVertex(pos);
	
	pos = p;
	pos.x -= s;
	pos.y += s;
	pos.z += s;
	cubeVertex(pos);
	
	pos = p;
	pos.x += s;
	pos.y -= s;
	pos.z += s;
	cubeVertex(pos);
	
	pos = p;
	pos.x -= s;
	pos.y -= s;
	pos.z += s;
	cubeVertex(pos);
	
	pos = p;
	pos.x -= s;
	pos.y -= s;
	pos.z -= s;
	cubeVertex(pos);
	
	pos = p;
	pos.x += s;
	pos.y -= s;
	pos.z -= s;
	cubeVertex(pos);
	
	pos = p;
	pos.x -= s;
	pos.y += s;
	pos.z -= s;
	cubeVertex(pos);
	
	pos = p;
	pos.x += s;
	pos.y += s;
	pos.z -= s;
	cubeVertex(pos);
	
	EndPrimitive();
}

void main()
{
	/* Clamp this to the range where it actually looks remotely good */
	float diff = max(min(1 - animPosition, 0.6), 0.18);
	float size = diff + (diff / 2);

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