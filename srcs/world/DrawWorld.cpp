#include "world/DrawWorld.hpp"

DrawWorld::DrawWorld() {}

DrawWorld::~DrawWorld() {}

void  DrawWorld::addChunk(const ChunkCoord& coord, const ChunkData& chunk)
{
  if (chunks_.find(coord) != chunks_.end())
    return ;

  chunks_[coord] = std::make_unique<ChunkData>(chunk);
}

const ChunkData*  DrawWorld::getChunk(const ChunkCoord& coord) const
{
  auto it = chunks_.find(coord);

  if (it == chunks_.end())
    return nullptr;

  return it->second.get();
}

BlockID DrawWorld::getBlock(int bx, int by, int bz) const
{
  ChunkCoord  coord = toChunkCoord(bx, by, bz);
  LocalCoord  local = toLocalCoord(bx, by, bz);

  const ChunkData*  chunk = getChunk(coord);
  if (chunk == nullptr)
    return (BLOCK_AIR);
  return (chunk->getBlockID(local.x, local.y, local.z));
}
