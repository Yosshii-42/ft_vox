#include "graphics/RenderHelpers.hpp"

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

void drawSkybox(Shader& skyboxShader, const SkyBox& skybox,
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

void        drawCrosshair(Shader& crosshairShader, const Crosshair& crosshair, float aspectRatio)
{
  glDisable(GL_DEPTH_TEST);
  crosshair.draw(crosshairShader, aspectRatio);
  glEnable(GL_DEPTH_TEST);  
}
