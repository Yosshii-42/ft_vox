#include "world/TerrainGenerator.hpp"

#include "voxel/ChunkData.hpp"

#include <cmath>
#include <algorithm>

TerrainGenerator::TerrainGenerator(Seed seed)
  : seed_(seed)
{
  (void)seed_;
}

void  TerrainGenerator::generateChunk(int cx, int cy, int cz, ChunkData& chunk)
{
#ifdef DEBUG_CHUNK_BOUNDARY
  (void)cx;
  (void)cy;
  (void)cz;
  for (int y = 0; y < VOXEL_CHUNK_SIZE; ++y)
  {
    for (int z = 0; z < VOXEL_CHUNK_SIZE; ++z)
    {
      for (int x = 0; x < VOXEL_CHUNK_SIZE; ++x)
      {
        chunk.setBlock(x, y, z, BLOCK_STONE);
      }
    }
  }
  return;
#else
  for (int y = 0; y < VOXEL_CHUNK_SIZE; ++y)
  {
    int worldY = cy * VOXEL_CHUNK_SIZE + y;
    for (int z = 0; z < VOXEL_CHUNK_SIZE; ++z)
    {
      int worldZ = cz * VOXEL_CHUNK_SIZE + z;
      for (int x = 0; x < VOXEL_CHUNK_SIZE; ++x)
      {
        BlockID id = BLOCK_AIR;
        int worldX = cx * VOXEL_CHUNK_SIZE + x;
        int groundHeight = getGroundHeight(worldX, worldZ);

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
        chunk.setBlock(x, y, z, id);
      }
    }
  }
#endif
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

  double  height = 20.0 + large + medium + small + mountain;

  return (static_cast<int>(std::floor(height)));
}
