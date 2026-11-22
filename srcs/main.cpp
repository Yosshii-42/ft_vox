#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <sstream>
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
#include "voxel/SkyBox.hpp"
#include "world/TerrainGenerator.hpp"
#include "world/DrawWorld.hpp"
#include "world/SunMoon.hpp"
#include "graphics/OpenGLSettings.hpp"
#include "graphics/RenderHelpers.hpp"
#include "TexturePaths.hpp"

int main()
{
  std::srand(static_cast<unsigned int>(std::time(NULL)));
  try
  {
    // window作成とOpenGL初期設定
    Window window;
    initOpenGLSettings();

    // shaderをインスタンス化 注)OpenGLコンテキストを使うのでWindowの後
    Shader  worldShader("shaders/world.vert", "shaders/world.frag");
    Shader  crosshairShader("shaders/crosshair.vert", "shaders/crosshair.frag");
    Shader  skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
    Shader  hudShader("shaders/hud.vert", "shaders/hud.frag");
    Crosshair crosshair;
    window.show();

    // camera作成
    Camera  camera;
    camera.setPosition(Vec3(16.0f, 35.0f, 16.0f));

    // 画像読み込み
    TextureArray  tex;
    tex.load(TexturePaths::terrain);
    SkyBox  skybox;
    skybox.load(TexturePaths::skybox);
    Hud     hud;
    hud.load(TexturePaths::fontset);

    // 地形作成
    TerrainGenerator  generator(12345);
    DrawWorld         world(generator);
    SunMoon           sunMoon;

    // 変数初期化
    RenderState       state;

    // projection行列
    float             aspectRatio = window.getAspectRatio();
    const Mat4        projection = camera.getProjectionMatrix(aspectRatio);

    // レンダリングループ 
    while (!window.shouldClose())
    {
      // time
      double  currentTime = glfwGetTime();
      float   deltaTime = static_cast<float>(currentTime - state.lastTime);
      state.lastTime = currentTime;

      // イベントとキー入力
      glfwPollEvents();
      window.processInput(camera, deltaTime, world, state);

      // カメラ向き更新
      world.setForward(camera.getForward());

      // 変更前のカメラ位置と描画距離を取得
      const ChunkCoord center = camera.getChunkCoord(camera.getPosition());
      int renderRadius = world.getRenderRadius();
      
      // チャンクを更新
      #ifndef DEBUG_CHUNK_BOUNDARY
        if (center != state.lastCenter ||
            renderRadius != state.lastRenderRadius)
        {
          world.loadAround(center);
          world.unloadFarChunks(center.x, center.z);

          state.lastCenter = center;
          state.lastRenderRadius = renderRadius;
        }
        world.collectLoadedChunks();  //　毎フレーム生成結果を回収
      #endif
      world.updateDirtyChunks(camera);  // 更新が必要なチャンクのメッシュを再生成
      
      // light位置更新
      sunMoon.update(deltaTime);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      // 描画
      Matrices    matrices = makeMatrices(camera, projection);
      RenderStats stats = drawWorld(worldShader, tex, world, matrices, center, sunMoon); 
      drawSkybox(skyboxShader, skybox, matrices, sunMoon);
      if (state.hudVisible == true)
      {
        drawHud(hudShader, hud, state.text, window.getWidth(), window.getHeight());
      }
      drawCrosshair(crosshairShader, crosshair, aspectRatio);
      
      // Calculate and display rendering statistics every second.
      ++state.frameCount;
      double  elapsed = glfwGetTime() - state.fpsLastTime;

      if (elapsed >= 1.0)
      {
        state.fps = static_cast<double>(state.frameCount) / elapsed;
        std::ostringstream  ossf;
        ossf << std::fixed << std::setprecision(1) << state.fps;
        std::string chunks = std::to_string(stats.chunks);
        std::string vertices = std::to_string(stats.vertices);
        state.memory = getMemoryUsageMiB();
        std::ostringstream  ossm;
        ossm << std::fixed << std::setprecision(1) << state.memory;
        
        state.text = "FPS: " + ossf.str() + "\n" +
                     "CHUNKS: " + chunks + "\n" +
                     "VERTICES: " + vertices + "\n" +
                     "MEMORY: " + ossm.str() + " MiB";

        state.frameCount = 0;
        state.fpsLastTime = glfwGetTime();
      }

      glfwSwapBuffers(window.getWindow());
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    exit (EXIT_FAILURE);
  } 
	return (EXIT_SUCCESS);
}
