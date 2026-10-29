// ブロックの配置情報
#pragma once

// #include "voxel/Block.hpp"
#include "voxel/VoxCommon.hpp"
#include <array>

class ChunkData {
private:
  std::array<BlockID, VOXEL_CHUNK_VOLUME> blocks_;

public:
  ChunkData();
  ~ChunkData() = default;

  BlockID getBlockID(int x, int y, int z) const;
  void    setBlock(int x, int y, int z, BlockID id);
  bool    isInside(int x, int y, int z) const;
};
