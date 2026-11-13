#version 330 core

out vec4          fragColor;

in vec2           TexCoord;
flat in int       Face;
flat in int       TexLayer;

uniform sampler2DArray ourTexture;
uniform vec3      ambientLight;
uniform float     ambientStrength;
uniform vec3      lightDir;
uniform float     sunLightStrength;
// uniform vec3      sunLightColor;

void  main()
{
  vec3  normal;
  switch (Face)
  {
    case 0: normal = vec3(-1, 0, 0); break;
    case 1: normal = vec3(1, 0, 0); break;
    case 2: normal = vec3(0, -1, 0); break;
    case 3: normal = vec3(0, 1, 0); break;
    case 4: normal = vec3(0, 0, -1); break;
    default: normal = vec3(0, 0, 1); break;
  }

  float diffuse = max(0, dot(normal, normalize(lightDir)));

  vec2  tileUV = fract(TexCoord);
  vec4  texColor = texture(ourTexture, vec3(tileUV, float(TexLayer)));
  vec3  result = texColor.rgb *
                (ambientLight * ambientStrength +
                 vec3(diffuse) * sunLightStrength);

  fragColor = vec4(result, texColor.a);
}
