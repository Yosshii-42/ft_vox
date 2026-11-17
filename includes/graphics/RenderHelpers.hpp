#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <limits>
#include "math/Mat4.hpp"
#include "gl/Camera.hpp"
#include "gl/Shader.hpp"
#include "gl/TextureArray.hpp"
#include "gl/Crosshair.hpp"
#include "world/DrawWorld.hpp"
#include "world/SunMoon.hpp"
#include "voxel/SkyBox.hpp"
#include "voxel/VoxCommon.hpp"

struct RenderState
{
  float       lastTime = static_cast<float>(glfwGetTime());
  ChunkCoord  lastCenter{
    std::numeric_limits<int>::max(),
    std::numeric_limits<int>::max(),
    std::numeric_limits<int>::max()   
  };
  int         lastRenderRadius = 0;
  #ifdef DEBUG_RENDER_STATS
    double  fpsLastTime = glfwGetTime();
    int     frameCount = 0;
  #endif
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
