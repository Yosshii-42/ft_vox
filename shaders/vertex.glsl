#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in float layer;
// layout (loaction = 3) in float face;

out vec2     TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float textureScale;

void  main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
//  ourColor = aColor;
  TexCoord = aUV * textureScale;
}
