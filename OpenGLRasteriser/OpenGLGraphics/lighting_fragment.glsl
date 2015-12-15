#version 330 core

uniform float animPosition;
uniform sampler2D objectTexture;

uniform vec3 cameraPos;

uniform vec3 lightColour[2];
uniform vec3 lightPos[2];
uniform float lightRadius[2];

in Vertex
{
  vec2 texCoord;
  vec4 colour;
  vec3 worldPos;
  vec3 normal;
} IN;

out vec4 fragCol;

mat4 processLight(int idx)
{
  vec3 incident = normalize(lightPos[idx] - IN.worldPos);
  vec3 viewDir = normalize(cameraPos - IN.worldPos);
  vec3 halfDir = normalize(incident + viewDir);

  float dist = length(lightPos[idx] - IN.worldPos);
  float atten = 1.0 - clamp(dist / lightRadius[idx], 0.0 , 1.0);
  float lambert = max(0.0, dot(incident, IN.normal));

  float rFactor = max(0.0, dot(halfDir, IN.normal));
  float sFactor = pow(rFactor, 100.0);

  vec4 texCol = texture(objectTexture, IN.texCoord);

  mat4 light;
  // Texture colour
  light[0] = texCol;
  // Ambient
  light[1].rgb = texCol.rgb * lightColour[idx] * 0.1;
  // Diffuse
  light[2].rgb = texCol.rgb * lightColour[idx] * lambert * atten;
  // Specular
  light[3].rgb = lightColour[idx] * sFactor * atten;

  return light;
}

void main(void)
{
  mat4 light1 = processLight(0);
  mat4 light2 = processLight(1);

  fragCol = vec4(light1[1].rgb + light1[2].rgb + light1[3].rgb + light2[3].rgb,
                 light1[0].a);
}
