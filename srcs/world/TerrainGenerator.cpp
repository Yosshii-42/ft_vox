#include "world/TerrainGenerator.hpp"

#include "voxel/ChunkData.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

TerrainGenerator::TerrainGenerator(Seed seed)
  : seed_(seed)
{
  (void)seed_;
}

void  TerrainGenerator::generateChunk(int cx, int cy, int cz,
                                      ChunkData& chunk) const
{
  const int trunkHeight = 5;
  int preTreeZ = std::numeric_limits<int>::max();
  std::array<int, 3> treeHeights{};

  for (int z = 0; z < VOXEL_CHUNK_SIZE; ++z)
  {
    int worldZ = cz * VOXEL_CHUNK_SIZE + z;
    int treeZ = static_cast<int>(std::floor((worldZ + 4) / 8.0)) * 8;
    int dz = std::abs(worldZ - treeZ);

    if (preTreeZ !=treeZ)
    {
      preTreeZ = treeZ;
      for (int i = 0; i < 3; ++i)
      {
        int treeX = cx * VOXEL_CHUNK_SIZE + 8 * i;
        treeHeights[i] = getGroundHeight(treeX, preTreeZ) + trunkHeight;
      }
    }
    
    for (int x = 0; x < VOXEL_CHUNK_SIZE; ++x)
    {
      int       worldX = cx * VOXEL_CHUNK_SIZE + x;        
      int       treeX = static_cast<int>(std::floor((worldX + 4) / 8.0)) * 8;
      int       index = (treeX - cx * VOXEL_CHUNK_SIZE) / 8;
      const int heightCenter = treeHeights[index];
      int       dx = std::abs(worldX - treeX);
      int       groundHeight = getGroundHeight(worldX, worldZ);
      
      for (int y = 0; y < VOXEL_CHUNK_SIZE; ++y)
      {
        BlockID id = BLOCK_AIR;
        int worldY = cy * VOXEL_CHUNK_SIZE + y;
        int radius = (worldY == heightCenter) ? 2 : 1;

        if (worldY == 0)
          id = BLOCK_BEDROCK;
        else if (worldY < groundHeight - 3 && worldY > 0)
          id = BLOCK_STONE;
        else if (worldY <= groundHeight && worldY > 0)
          id = BLOCK_DIRT;

        // テスト用の空洞
        if (worldX >= 8 && worldX < 24 &&
            worldY >= 3 && worldY < 20 &&
            worldZ >= 8 && worldZ < 24)
        {
          id = BLOCK_AIR;
        }

        // wood
        if (dx == 0 && dz == 0 && groundHeight < worldY && worldY <= groundHeight + trunkHeight)
        {
          id = BLOCK_WOOD;
        }
        if (id == BLOCK_AIR && heightCenter - 1 <= worldY && worldY <= heightCenter + 1)
        {
          if (!(dx == 2 && dz == 2) && dx <= radius && dz <= radius)
            id = BLOCK_LEAVES;
        }

        if (id != BLOCK_AIR)
          chunk.setBlock(x, y, z, id);
      }
    }
  }
}

int TerrainGenerator::getGroundHeight(int worldX, int worldZ) const
{
  double  x = static_cast<double>(worldX);
  double  z = static_cast<double>(worldZ);

  double  large = perlin_.noise(x * 0.01, z * 0.01) * 16.0;
  double  medium = perlin_.noise(x * 0.03, z * 0.03) * 8.0;
  double  small = perlin_.noise(x * 0.09, z * 0.09) * 3.0;

  // 山岳地帯
  double  mountainNoise = perlin_.noise(x * 0.003, z * 0.003);

  double  mountain = std::max(0.0, mountainNoise - 0.1) * 120.0;

  double  height = 85.0 + large + medium + small + mountain;

  return (static_cast<int>(std::floor(height)));
}
