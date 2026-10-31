#version 330 core

out vec4          fragColor;

in vec2           TexCoord;
flat in int       TexLayer;

uniform sampler2DArray ourTexture;

void  main()
{
  vec2 tileUV = fract(TexCoord);

  fragColor = texture(
    ourTexture,
    vec3(tileUV, float(TexLayer))
  );
}
