#include "graphics/OpenGLSettings.hpp"
#include <glad/glad.h>

void  initOpenGLSettings()
{
  glEnable(GL_DEPTH_TEST);                            // 深度テストを有効にする
  glEnable(GL_BLEND);                                 // ブレンディングを有効にする
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // どうブレンドするかを指定する
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);               // 背景色   
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);          // 描画mode設定
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glDisable(GL_CULL_FACE);                            // 裏面処理
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}