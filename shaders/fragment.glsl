#version 330 core

out vec4          fragColor;

in vec2           TexCoord;
flat in int       TexLayer;

uniform sampler2DArray ourTexture;
uniform vec3      ambientLight;
uniform float     ambientStrength;

void  main()
{
  vec2 tileUV = fract(TexCoord);
  vec4 texColor = texture(ourTexture, vec3(tileUV, float(TexLayer)));
  vec3 result = texColor.rgb * ambientLight * ambientStrength;

  fragColor = vec4(result, texColor.a);
}
