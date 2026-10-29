#version 330 core

out vec4          fragColor;

in vec2           TexCoord;

uniform sampler2D ourTexture;
uniform bool      useTexture;
uniform float     textureBlend;

void  main()
{
  vec4 texColor = texture(ourTexture, TexCoord);
  fragColor = mix(vec4(1.0, 0.0, 0.0, 1.0), texColor, textureBlend);
}
