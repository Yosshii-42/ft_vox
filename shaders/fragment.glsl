#version 330 core

out vec4          fragColor;

in vec2           TexCoord;
flat in int       TexLayer;

uniform sampler2DArray ourTexture;
uniform bool      useTexture;
uniform float     textureBlend;

void  main()
{
//  vec4 texColor = texture(ourTexture, vec3(TexCoord, float(TexLayer)));
//  fragColor = mix(vec4(1.0, 0.0, 0.0, 1.0), texColor, textureBlend);

  fragColor = texture(ourTexture, vec3(TexCoord, float(TexLayer)));
//  if (TexLayer == 0)
//    fragColor = vec4(0.55, 0.35, 0.20, 1.0);  // DIRT
//  else if (TexLayer == 1)
//    fragColor = vec4(0.55, 0.55, 0.55, 1.0);  // STONE
//  else if (TexLayer == 2)
//    fragColor = vec4(0.30, 0.30, 0.35, 1.0);  // BEDROCK
//  else if (TexLayer == 3)
//    fragColor = vec4(0.90, 0.80, 0.55, 1.0);  // SAND
//  else if (TexLayer == 4)
//    fragColor = vec4(0.95, 0.95, 1.00, 1.0);  // SNOW
//  else
//    fragColor = vec4(1.00, 0.00, 1.00, 1.0);  // その他
}
