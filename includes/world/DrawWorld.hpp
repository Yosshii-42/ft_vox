#pragma once

#include "voxel/ChunkData.hpp"
#include "voxel/VoxCommon.hpp"

#include <map>
#include <memory>

class DrawWorld {
private:
  std::map<ChunkCoord, std::unique_ptr<ChunkData>> chunks_;

public:
  DrawWorld();
  ~DrawWorld();
  DrawWorld(const DrawWorld& other) = delete;
  DrawWorld& operator=(const DrawWorld& other) = delete;

  void              addChunk(const ChunkCoord& coord, const ChunkData& chunk);
  const ChunkData*  getChunk(const ChunkCoord& coord) const;
  BlockID           getBlock(int bx, int by, int bz) const;
};
