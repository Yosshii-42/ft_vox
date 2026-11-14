#version 330 core

in vec3 TexCoords;

out vec4  fragColor;

uniform samplerCube ourTexture;

void main()
{
  vec4  color = texture(ourTexture, TexCoords);
  fragColor = color;
}
