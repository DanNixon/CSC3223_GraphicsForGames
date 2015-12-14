#version 330 core

uniform float animPosition;
uniform sampler2D objectTexture;

uniform vec3 cameraPos;

uniform vec3 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex
{
	vec2 texCoord;
	vec4 colour;
	vec3 worldPos;
	vec3 normal;
} IN;

out vec4 fragCol;

void main(void)
{
	vec3 incident = normalize(lightPos - IN.worldPos);
  vec3 viewDir = normalize(cameraPos - IN.worldPos);
  vec3 halfDir = normalize(incident + viewDir);

  float dist = length(lightPos - IN.worldPos);
  float atten = 1.0 - clamp(dist / lightRadius, 0.0 , 1.0);
  float lambert = max(0.0, dot(incident, IN.normal));

  float rFactor = max(0.0, dot(halfDir, IN.normal));
  float sFactor = pow(rFactor , 50.0);

  vec4 texCol = texture(objectTexture, IN.texCoord);
  vec3 ambient = texCol.rgb * lightColour * 0.1;
  vec3 diffuse = texCol.rgb * lightColour * lambert * atten;
  vec3 specular = lightColour * sFactor * atten;
	
  fragCol = vec4(ambient + diffuse + specular, texCol.a);
}