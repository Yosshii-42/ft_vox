#version 330 core

layout (location = 0) in vec3   aPos;
layout (location = 1) in vec2   aUV;
layout (location = 2) in float  aLayer;
layout (location = 3) in float  aFace;

out vec2      TexCoord;
flat out int  TexLayer; // flatは補完方法指定
flat out int  Face;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform float textureScale;

void  main()
{
  Face = int(aFace);
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  TexCoord = aUV;
  TexLayer = int(aLayer);
}

// flat: 三角形内で値を補完しない
