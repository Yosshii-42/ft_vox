#version 330 core

in vec3 TexCoords;

out vec4  fragColor;

uniform samplerCube ourTexture;
uniform float       sunLightStrength;

void main()
{
  vec4  color = texture(ourTexture, TexCoords);
  // vec3  result = color.rbg * sunLightStrength;
  // fragColor = vec4(result, color.a);
  fragColor = color;
}
