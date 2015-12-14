#version 330 core

uniform float animPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (triangles) in;
layout (line_strip, max_vertices = 256) out;

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
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * pos;
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
	float size = diff + (diff / 4);

	for (float s = 0; s < 1; s += diff)
	{
		for (float t = 0; t < 1; t += diff)
		{
			if (s + t > 1)
				continue;

			//cubeAtPoint(triPos(s, t), size);
		}
	}
	
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;
	
	vec3 v0 = p0 - p1;
	vec3 v1 = p2 - p1;
	
	vec3 norm = cross(v1, v0);
	norm = normalize(norm);
	
	vec3 p = (p0 + p1 + p2) / 3;
	
	OUT.colour = vec4(1, 0, 0, 1);
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(p, 1);
	EmitVertex();
	OUT.colour = vec4(0, 1, 0, 1);
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(p + norm, 1);
	EmitVertex();
	EndPrimitive();
}