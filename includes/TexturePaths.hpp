#pragma once

#include <string>
#include <vector>

namespace TexturePaths
{
  const std::vector<std::string> terrain = {
    "textures/dirt.png",
    "textures/stone.png",
    "textures/bedrock.png",
    "textures/dirt_top.png"
  };

  const std::vector<std::string> skybox = {
    "textures/clouds1_east.bmp",
    "textures/clouds1_west.bmp",
    "textures/clouds1_up.bmp",
    "textures/clouds1_down.bmp",
    "textures/clouds1_north.bmp",
    "textures/clouds1_south.bmp"   
  };

  const std::string fontset =
    "textures/fontset.png";
}
