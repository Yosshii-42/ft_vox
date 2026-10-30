#version 330 core

out vec4          fragColor;

in vec2           TexCoord;
flat in int       TexLayer;

uniform sampler2DArray ourTexture;

void  main()
{
  fragColor = texture(
    ourTexture,
    vec3(TexCoord, float(TexLayer))
  );
}
