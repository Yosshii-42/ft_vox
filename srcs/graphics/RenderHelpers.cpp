#include "graphics/RenderHelpers.hpp"
#include "world/DrawWorld.hpp"
#ifdef __APPLE__
#include <mach/mach.h>
#endif

Matrices  makeMatrices(const Camera& camera, const Mat4& projection)
{
  Matrices  matrices;

  matrices.view = camera.getViewMatrix();
  matrices.projection = projection;
  matrices.skyboxView = camera.getSkyboxViewMatrix();

  return matrices;
}

RenderStats drawWorld(Shader& shader, const TextureArray& tex,
                      const DrawWorld& world, const Matrices& matrices,
                      const ChunkCoord& center, const SunMoon& sunMoon)
{
  shader.use();
  // texture
  shader.setInt("ourTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  tex.bind();
  // light
  shader.setVec3("ambientLight", Common::AMBIENT_LIGHT);
  Vec3  sunDir = sunMoon.getSunDirection();
  shader.setFloat("ambientStrength", sunMoon.getAmbientStrength(sunDir));
  shader.setVec3("sunLightDir", sunDir);
  shader.setFloat("sunLightStrength", sunMoon.getSunLightStrength(sunDir));
  // draw
  RenderStats stats = world.draw(shader,
                                 matrices.model, matrices.view, matrices.projection,
                                 center.x, center.z);
  
  return stats;
}

void  drawSkybox(Shader& skyboxShader, const SkyBox& skybox,
                const Matrices& matrices, const SunMoon& sunMoon)
{
  skyboxShader.use();
  // texture
  skyboxShader.setInt("ourTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  // light
  Vec3  sunDir = sunMoon.getSunDirection();
  skyboxShader.setMat4("view", matrices.skyboxView);
  skyboxShader.setMat4("projection", matrices.projection);
  skyboxShader.setFloat("sunLightStrength", sunMoon.getSkyLightStrength(sunDir));
  // draw
  skybox.draw(skyboxShader);
}

void  drawCrosshair(Shader& crosshairShader, const Crosshair& crosshair, float aspectRatio)
{
  glDisable(GL_DEPTH_TEST);
  crosshair.draw(crosshairShader, aspectRatio);
  glEnable(GL_DEPTH_TEST);  
}

void  drawHud(Shader& hudShader, const Hud& hud, const std::string& text,
              int screenWidth, int screenHeight)
{
  hudShader.use();
  // texture
  hudShader.setInt("fontTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  hud.bind();
  // 行列作成
  Mat4 projection = Mat4::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
  hudShader.setMat4("projection", projection);

  glDisable(GL_DEPTH_TEST);
  hud.draw(hudShader, text, 20.0f, 20.0f, 8.0f, 16.0f);
  glEnable(GL_DEPTH_TEST);
}

double  getMemoryUsageMiB()
{
#ifdef __APPLE__
  task_vm_info_data_t info{};
  mach_msg_type_number_t  count = TASK_VM_INFO_COUNT;
  kern_return_t result = task_info(
    mach_task_self(),
    TASK_VM_INFO,
    reinterpret_cast<task_info_t>(&info),
    &count
  );
  if (result != KERN_SUCCESS)
    return 0.0;
  double  memoryMiB = static_cast<double>(info.phys_footprint) / (1024.0 * 1024.0);
  return memoryMiB;
#elif defined(__linux__)
  // todo
#endif
  return 0.0;
}

// Calculate and display rendering statistics every second.
void  updateRenderState(RenderState& state, double currentTime,
                        const DrawWorld& world, const RenderStats& stats)
{
  ++state.frameCount;
  double  elapsed = currentTime - state.fpsLastTime;

  if (elapsed < 1.0)
    return;

  state.fps = static_cast<double>(state.frameCount) / elapsed;
  std::ostringstream  ossf;
  ossf << std::fixed << std::setprecision(1) << state.fps;
  std::string chunks = std::to_string(stats.chunks);
  state.chunksCount = world.getLoaderChunkCount();
  std::string loaderChunkCount = std::to_string(state.chunksCount);
  std::string vertices = std::to_string(stats.vertices);
  state.memory = getMemoryUsageMiB();
  std::ostringstream  ossm;
  ossm << std::fixed << std::setprecision(1) << state.memory;
  state.requestsCount = world.getRequestsCount();
  state.resultsCount = world.getResultsCount();
  std::string requests = std::to_string(state.requestsCount);
  std::string results = std::to_string(state.resultsCount);

  state.text = "FPS: " + ossf.str() + "\n" +
                "CHUNKS: " + chunks + "\n" +
                "LOADER CHUNKS COUNT: " + loaderChunkCount + "\n" +
                "VERTICES: " + vertices + "\n" +
                "MEMORY: " + ossm.str() + " MiB" + "\n" +
                "REQUESTS: " + requests + "\n" +
                "RESULTS: " + results;

  state.frameCount = 0;
  state.fpsLastTime = currentTime;
}

