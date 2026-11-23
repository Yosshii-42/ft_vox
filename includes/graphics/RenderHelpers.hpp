#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <limits>
#include "math/Mat4.hpp"
#include "gl/Camera.hpp"
#include "gl/Shader.hpp"
#include "gl/TextureArray.hpp"
#include "gl/Crosshair.hpp"
// #include "world/DrawWorld.hpp"
#include "world/SunMoon.hpp"
#include "voxel/SkyBox.hpp"
#include "voxel/VoxCommon.hpp"
#include "graphics/Hud.hpp"

class DrawWorld;
struct RenderStats;

struct RenderState
{
  float       lastTime = static_cast<float>(glfwGetTime());
  ChunkCoord  lastCenter{
    std::numeric_limits<int>::max(),
    std::numeric_limits<int>::max(),
    std::numeric_limits<int>::max()   
  };
  int         lastRenderRadius = 0;
  double      fpsLastTime = glfwGetTime();
  int         frameCount = 0;
  double      fps = 0.0;
  bool        hudVisible = true;
  bool        is4KeyPressed = false;
  double      memory = 0.0;
  size_t      requestsCount = 0;
  size_t      resultsCount = 0;
  size_t      chunksCount = 0;
  std::string text;
};

struct Matrices
{
  Mat4  model;
  Mat4  view;
  Mat4  projection;
  Mat4  skyboxView;
};

Matrices    makeMatrices(const Camera& camera, const Mat4& projection);
RenderStats drawWorld(Shader& shader, const TextureArray& tex,
                      const DrawWorld& world, const Matrices& matrices,
                      const ChunkCoord& center, const SunMoon& sunMoon);
void        drawSkybox(Shader& skyboxShader, const SkyBox& skybox,
                       const Matrices& matrices, const SunMoon& sunMoon);
void        drawCrosshair(Shader& crosshairShader, const Crosshair& crosshair,
                          float aspectRatio);
void        drawHud(Shader& hudShader, const Hud& hud, const std::string& text,
                    int screenWidth, int screenHeight);
double      getMemoryUsageMiB();
void        updateRenderState(RenderState& state, double currentTime,
                              const DrawWorld& world, const RenderStats& stats);
