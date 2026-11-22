#version 330 core

in vec2 texCoord;

out vec4  FragColor;

uniform sampler2D fontTexture;

void main()
{
  vec4  texColor = texture(fontTexture, texCoord);
  FragColor = vec4(1.0, 1.0, 1.0, texColor.a);
}
