# version 400 core

layout (triangles, equal_spacing, cw) in;

uniform sampler2D objectTextures[5];

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in Vertex
{
  vec2 texCoord;
  vec4 colour;
} IN[];

out Vertex
{
  vec2 texCoord;
  vec4 colour;
  vec3 worldPos;
  vec3 normal;
} OUT;

vec3 TriMixVec3(vec3 a, vec3 b, vec3 c)
{
  vec3 p0 = gl_TessCoord.x * a;
  vec3 p1 = gl_TessCoord.y * b;
  vec3 p2 = gl_TessCoord.z * c;

  return p0 + p1 + p2;
}

vec2 TriMixVec2(vec2 a, vec2 b, vec2 c)
{
  vec2 p0 = gl_TessCoord.x * a;
  vec2 p1 = gl_TessCoord.y * b;
  vec2 p2 = gl_TessCoord.z * c;

  return p0 + p1 + p2;
}

void main ()
{
  vec3 combinedPos = TriMixVec3(
      gl_in[0].gl_Position.xyz,
      gl_in[1].gl_Position.xyz,
      gl_in[2].gl_Position.xyz);

  OUT.texCoord = TriMixVec2(
      IN[0].texCoord,
      IN[1].texCoord,
      IN[2].texCoord);

  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  vec3 p2 = gl_in[2].gl_Position.xyz;

  vec3 v0 = p0 - p1;
  vec3 v1 = p2 - p1;

  vec3 norm = cross(v1, v0);
  norm = normalize(norm);

  vec4 pos = vec4(combinedPos, 1);

  vec4 texCol = texture(objectTextures[2], OUT.texCoord);
  float height = texCol.r + texCol.g + texCol.g;
  if (height > 2.0)
  {
    height /= 30;
    pos.xyz -= (norm * vec3(height, height, height));
  }

  vec4 worldPos = modelMatrix * pos;
  OUT.worldPos = worldPos.xyz;
  OUT.normal = norm;

  gl_Position = projMatrix * viewMatrix * worldPos;
}
