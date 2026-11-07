#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "gl/Shader.hpp"
#include "gl/Window.hpp"
#include "gl/TextureArray.hpp"
#include "gl/Camera.hpp"
#include "gl/Crosshair.hpp"
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
    // glDisable(GL_CULL_FACE);                            // 裏面処理
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
   
    // shaderをインスタンス化 注)OpenGLコンテキストを使うのでWindowの後
    Shader  shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Shader  crosshairShader("shaders/crosshair.vert", "shaders/crosshair.frag");
    Crosshair crosshair;

    // shaderが正常にコンパイルされたらwindow表示する
    window.show();

    // camera作成
    Camera  camera;
    camera.setPosition(Vec3(16.0f, 35.0f, 16.0f));

    TextureArray  tex;
    tex.load({
      "textures/dirt.png",
      "textures/stone.png",
      "textures/bedrock.png"});
    
    DrawWorld world;
    TerrainGenerator  generator(12345);

    #ifdef DEBUG_CHUNK_BOUNDARY
      world.testChunkBoundary(generator);
    #else
      const Vec3& pos = camera.getPosition();
      int cx = floorDiv(static_cast<int>(std::floor(pos.x)));
      int cz = floorDiv(static_cast<int>(std::floor(pos.z)));

      world.loadAround(cx, cz, generator);
    #endif

    float textureScale = 1.0f;
    float lastTime = glfwGetTime();
    int   lastCx = std::numeric_limits<int>::max();
    int   lastCz = std::numeric_limits<int>::max();
    int   lastRenderRadius = world.getRenderRadius();
    #ifdef DEBUG_RENDER_STATS
      double  fpsLastTime = glfwGetTime();
      int   frameCount = 0;
    #endif

    // レンダリングループ 
    while (!window.shouldClose())
    {
      double  currentTime = glfwGetTime();
      float   deltaTime = static_cast<float>(currentTime - lastTime);
      lastTime = currentTime;

      // ① イベントとキー入力
      glfwPollEvents();
      window.processInput(textureScale, camera, deltaTime, world);

      // ② 変更前のカメラ位置と描画距離を取得
      const Vec3& pos = camera.getPosition();
      int cx = floorDiv(static_cast<int>(std::floor(pos.x)));
      int cz = floorDiv(static_cast<int>(std::floor(pos.z)));

      int renderRadius = world.getRenderRadius();
      
      // ③ チャンクを更新
      if (cx != lastCx || cz != lastCz || renderRadius != lastRenderRadius)
      {
        #ifndef DEBUG_CHUNK_BOUNDARY
          world.loadAround(cx, cz, generator);
          world.unloadFarChunks(cx, cz);
        #endif

        lastCx = cx;
        lastCz = cz;
        lastRenderRadius = renderRadius;
      }

      // 更新が必要なチャンクのメッシュを再生成  
      world.updateDirtyChunks();
      
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
      Mat4  baseModel;
      Mat4  view = camera.getViewMatrix();
      Mat4  projection = camera.getProjectionMatrix(window.getAspectRatio());
      
      // 描画
      RenderStats stats =
        world.draw(shader, baseModel, view, projection, cx, cz);

      // 十字表示
      glDisable(GL_DEPTH_TEST);
      crosshair.draw(crosshairShader, window.getAspectRatio());
      glEnable(GL_DEPTH_TEST);

      // swap buffers and poll IO events
      glfwSwapBuffers(window.getWindow());

      #ifdef DEBUG_RENDER_STATS
      // Calculate and display rendering statistics every second.

      ++frameCount;

      double  elapsed = glfwGetTime() - fpsLastTime;

      if (elapsed >= 1.0)
      {
        double  fps = static_cast<double>(frameCount) / elapsed;
        std::cout << "FPS: " << fps
                  << " | Vertices: " << stats.vertices
                  << " | Draw Chunks: " << stats.chunks << std::endl;
                  frameCount = 0;
                  fpsLastTime = glfwGetTime();
      }
      #else
        (void)stats;
      #endif
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit (EXIT_FAILURE);
  } 
	return (EXIT_SUCCESS);
}


