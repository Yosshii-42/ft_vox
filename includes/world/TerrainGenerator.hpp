// どの座標に何のブロックを置くか決定
#pragma once

#include "voxel/VoxCommon.hpp"
#include "world/PerlinNoise.hpp"

class ChunkData;

class TerrainGenerator {
private:
  Seed        seed_;
  PerlinNoise perlin_;

  int getGroundHeight(int worldX, int worldZ) const;

public:
  TerrainGenerator() = delete;
  explicit TerrainGenerator(Seed seed);
  ~TerrainGenerator() = default;

  void  generateChunk(int cx, int cy, int cz,
                      ChunkData& chunk) const;
};
