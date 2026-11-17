#version 330 core

layout (location = 0) in vec3 aPos;

out vec3  TexCoords;

uniform mat4  view;
uniform mat4  projection;

void  main()
{
  TexCoords = aPos;
  vec4  pos = projection * view * vec4(aPos, 1.0);
  gl_Position = pos.xyww; // zにwを代入 深度z/1=1.0
}
