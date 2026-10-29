#include "voxel/ChunkData.hpp"

#include <stdexcept>

ChunkData::ChunkData() {
  blocks_.fill(BLOCK_AIR); // 空気で初期化する
}

BlockID ChunkData::getBlockID(int x, int y, int z) const
{
  if (isInside(x, y, z) == false)
    return (BLOCK_AIR);

  return (blocks_[blockIndex(x, y, z)]);
}

void  ChunkData::setBlock(int x, int y, int z, BlockID id)
{
  if (isInside(x, y, z) == false)
    throw std::out_of_range("Block coordinates out of range");

  blocks_[blockIndex(x, y, z)] = id;
}

bool  ChunkData::isInside(int x, int y, int z) const
{
  return (0 <= x && x < VOXEL_CHUNK_SIZE &&
          0 <= y && y < VOXEL_CHUNK_SIZE &&
          0 <= z && z < VOXEL_CHUNK_SIZE);
}
