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
#include "voxel/SkyBox.hpp"
#include "world/TerrainGenerator.hpp"
#include "world/DrawWorld.hpp"
#include "graphics/OpenGLSettings.hpp"
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
    Shader  shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Shader  crosshairShader("shaders/crosshair.vert", "shaders/crosshair.frag");
    Shader  skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
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

    // 地形作成
    TerrainGenerator  generator(12345);
    DrawWorld world(generator);

    #ifdef DEBUG_CHUNK_BOUNDARY
      world.testChunkBoundary(generator);
    #else
      const Vec3& pos = camera.getPosition();
      int cx = floorDiv(static_cast<int>(std::floor(pos.x)));
      int cy = floorDiv(static_cast<int>(std::floor(pos.y)));
      int cz = floorDiv(static_cast<int>(std::floor(pos.z)));

      world.loadAround(cx, cy, cz);
    #endif

    float textureScale = 1.0f;
    float lastTime = glfwGetTime();
    int   lastCx = std::numeric_limits<int>::max();
    int   lastCy = std::numeric_limits<int>::max();
    int   lastCz = std::numeric_limits<int>::max();
    int   lastRenderRadius = world.getRenderRadius();
    float sunAngle = 0.0f;
    float sunSpeed = 0.1f;
    // float sunLightStrength = 0.25f;
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
      int cy = floorDiv(static_cast<int>(std::floor(pos.y)));
      int cz = floorDiv(static_cast<int>(std::floor(pos.z)));

      int renderRadius = world.getRenderRadius();
      
      // ③ チャンクを更新
      #ifndef DEBUG_CHUNK_BOUNDARY
        if (cx != lastCx || cy != lastCy || cz != lastCz ||
            renderRadius != lastRenderRadius)
        {
          world.loadAround(cx, cy, cz);
          world.unloadFarChunks(cx, cz);

          lastCx = cx;
          lastCy = cy;
          lastCz = cz;
          lastRenderRadius = renderRadius;
        }
        //　毎フレーム生成結果を回収
        world.collectLoadedChunks();
      #endif

      // ④　更新が必要なチャンクのメッシュを再生成
      world.updateDirtyChunks(camera);
      
      // 毎フレーム画面と深度バッファをクリア
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      // ⑤ GPUに渡す / 使用するshaderprogramを指定し、作った行列をuniformに送る
      shader.use();
        // texture
      shader.setInt("ourTexture", 0);
      glActiveTexture(GL_TEXTURE0);
      tex.bind();
      shader.setFloat("textureScale", textureScale);
        // light
      sunAngle += sunSpeed * deltaTime;
      float sunX = std::cos(sunAngle);
      float sunY = std::sin(sunAngle);
      float sunZ = 0.0f;
      Vec3  sunLightDirection = Vec3(sunX, sunY, sunZ);
      float sunLightStrength = std::max(0.0f, sunY * 0.25f);
      float ambientStrength = 0.1f + std::max(0.0f, sunY) * 0.65f;
      shader.setVec3("ambientLight", Common::AMBIENT_LIGHT);
      shader.setFloat("ambientStrength", ambientStrength);
      shader.setVec3("sunLightDir", sunLightDirection);
      shader.setFloat("sunLightStrength", sunLightStrength);

      // 行列
      Mat4  baseModel;
      Mat4  view = camera.getViewMatrix();
      Mat4  projection = camera.getProjectionMatrix(window.getAspectRatio());
      
      // ⑥ ワールド描画
      RenderStats stats =
        world.draw(shader, baseModel, view, projection, cx, cz);

      // ⑦ Skybox描画
      Mat4  skyboxView = camera.getSkyboxViewMatrix();
      Mat4  skyboxProjection = camera.getProjectionMatrix(window.getAspectRatio());
      skyboxShader.use();
      skyboxShader.setInt("ourTexture", 0);
      glActiveTexture(GL_TEXTURE0);
      skyboxShader.setMat4("view", skyboxView);
      skyboxShader.setMat4("projection", skyboxProjection);
      skybox.draw(skyboxShader);

      // ⑧ 十字表示
      glDisable(GL_DEPTH_TEST);
      crosshair.draw(crosshairShader, window.getAspectRatio());
      glEnable(GL_DEPTH_TEST);

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


