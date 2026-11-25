#pragma once

#include <string>
#include <vector>

namespace TexturePaths
{
  const std::vector<std::string> terrain = {
    "textures/dirt.png",      // 0
    "textures/stone.png",     // 1
    "textures/bedrock.png",   // 2
    "textures/dirt_top.png",  // 3
    "textures/wood.png",      // 4
    "textures/wood_top.png",  // 5
    "textures/leaves.png"     // 6
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
