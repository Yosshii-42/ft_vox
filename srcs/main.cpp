#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "utils/utils.hpp"
#include "parser/Tokenizer.hpp"
#include "parser/BMP.hpp"
#include "gl/Shader.hpp"
#include "gl/Window.hpp"
#include "gl/Object.hpp"
#include "gl/Texture.hpp"
#include "gl/Manipulator.hpp"
#include "gl/Camera.hpp"
#include "gl/Instance.hpp"
#include "math/Vec3.hpp"
#include "validate/MeshDiagnostics.hpp"
#include "voxel/ChunkMesh.hpp"

std::vector<Vertex>  createTestFace()
{
  std::vector<Vertex> vertices;

  vertices.push_back({0, 0, 0.5, 0, 0, 0, 5});  // A
  vertices.push_back({0.5, 0, 0.5, 1, 0, 0, 5});  // B
  vertices.push_back({0.5, 0.5, 0.5, 1, 1, 0, 5});  // C

  vertices.push_back({0.5, 0.5, 0.5, 1, 1, 0, 5});  // C
  vertices.push_back({0, 0.5, 0.5, 0, 1, 0, 5});  // D
  vertices.push_back({0, 0, 0.5, 0, 0, 0, 5});  // A

  return (vertices);
}

// void  addFace(std::vector<Vertex>& vetices, int x, int y, int z, int face)
// {

// }

// // -X +X -Y +Y -Z +Z 各面のテクスチャレイヤー番号
// std::vector<Vertex> createTestBlock()
// {
//   std::vector<Vertex> vertices;

//   addFace(vertices, 0, 0, 0, 5); // z面

//   return (vertices);
// }

int main()
{
  std::srand(static_cast<unsigned int>(std::time(NULL)));
  try
  { 
    // window作成とOpenGL初期設定
    Window window;
    glEnable(GL_DEPTH_TEST);                            // 深度テストを有効にする
    glEnable(GL_BLEND);                                 // ブレンディングを有効にする
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // どうブレンドするかを指定する
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);               // 背景色   
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);          // 描画mode設定
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);                            // 裏面処理
    // glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
   
    // shaderをインスタンス化 注)OpenGLコンテキストを使うのでWindowの後
    Shader  shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    // shaderが正常にコンパイルされたらwindow表示する
    window.show();
    
    // camera作成
    Camera  camera;

    ChunkMesh mesh;
    mesh.upload(createTestFace());

    // レンダリングループ 
    while (!window.shouldClose())
    {

      // 毎フレーム画面と深度バッファをクリア
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // GPUに渡す / 使用するshaderprogramを指定し、作った行列をuniformに送る
      shader.use();
      // shader.setBool("useTexture", inputFlags.useTexture);
      // shader.setFloat("textureScale", textureScale);
      // shader.setFloat("textureBlend", textureBlend);
    
      // CPU側で変換行列を作って渡す modelは作りながら渡すので最後
      Mat4  view = camera.getViewMatrix();
      Mat4  projection = camera.getProjectionMatrix(window.getAspectRatio());
      Mat4  model;
      shader.setMatrices(model, view, projection);
      // 描画
      mesh.draw();

      // swap buffers and poll IO events
      glfwSwapBuffers(window.getWindow());
      glfwPollEvents();
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit (EXIT_FAILURE);
  } 
	return (EXIT_SUCCESS);
}


