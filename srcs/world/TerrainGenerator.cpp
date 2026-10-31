#include "world/TerrainGenerator.hpp"

#include "voxel/ChunkData.hpp"

#include <cmath>

TerrainGenerator::TerrainGenerator(Seed seed)
  : seed_(seed)
{
  (void)seed_;
}

void  TerrainGenerator::generateChunk(int cx, int cy, int cz, ChunkData& chunk)
{
  (void)cz;
  (void)cx;
  for (int y = 0; y < VOXEL_CHUNK_SIZE; ++y)
  {
    int worldY = cy * VOXEL_CHUNK_SIZE + y;
    for (int z = 0; z < VOXEL_CHUNK_SIZE; ++z)
    {
      // int worldZ = cz * VOXEL_CHUNK_SIZE + z;
      for (int x = 0; x < VOXEL_CHUNK_SIZE; ++x)
      {
        BlockID id = BLOCK_AIR;
        // int worldX = cx * VOXEL_CHUNK_SIZE + x;
        // double  groundHeight = static_cast<int>(
        //           35.0 +
        //           std::sin(worldX * 0.4) * 12.0 +
        //           std::sin(worldZ * 0.4) * 12.0);
        // double  groundHeight = 4 + (worldX / 4);

        if (worldY == 0)
          id = BLOCK_BEDROCK;
        else if (worldY < 5 && worldY > 0)
          id = BLOCK_STONE;
        else if (worldY < 8 && worldY >= 5)
          id = BLOCK_DIRT; 
        chunk.setBlock(x, y, z, id);
      }
    }
  }
}
