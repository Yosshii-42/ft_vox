#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "gl/Shader.hpp"
#include "gl/Window.hpp"
#include "gl/TextureArray.hpp"
#include "gl/Manipulator.hpp"
#include "gl/Camera.hpp"
#include "math/Vec3.hpp"
#include "voxel/ChunkMesh.hpp"
#include "voxel/ChunkData.hpp"
#include "voxel/VoxCommon.hpp"
#include "world/TerrainGenerator.hpp"
#include "world/DrawWorld.hpp"

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

    Manipulator manipulator(Vec3(0, 0, 0));
    
    // camera作成
    Camera  camera;

    TextureArray  tex;
    tex.load({
      "textures/dirt.png",
      "textures/stone.png",
      "textures/bedrock.png"});
    
    DrawWorld world;
    TerrainGenerator  generator(12345);

    for (int cy = 0; cy < 2; ++cy)
    {
      for (int cz = 0; cz < 2; ++cz)
      {
        for (int cx = 0; cx < 2; ++cx)
        {
          ChunkData chunk;

          generator.generateChunk(cx, cy, cz, chunk);
          world.addChunk({cx, cy, cz}, chunk);
        }
      }
    }
    world.buildAllMeshes();
    
    // input flagをインスタンス化
    Common::InputFlags  inputFlags;
    float   textureScale = 1.0f;

    // レンダリングループ 
    while (!window.shouldClose())
    {
      // キー操作
      window.processInput(manipulator, textureScale);
      
      // 毎フレーム画面と深度バッファをクリア
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      // GPUに渡す / 使用するshaderprogramを指定し、作った行列をuniformに送る
      shader.use();
      shader.setInt("ourTexture", 0);

      // texture
      glActiveTexture(GL_TEXTURE0);
      tex.bind();
      shader.setFloat("textureScale", textureScale);

      // 行列
      Mat4  baseModel = manipulator.getModelMatrix();
      Mat4  view = camera.getViewMatrix();
      Mat4  projection = camera.getProjectionMatrix(window.getAspectRatio());
      
      // 描画
      world.draw(shader, baseModel, view, projection);

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


