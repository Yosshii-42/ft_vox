#include "voxel/VoxCommon.hpp"

bool  operator==(const ChunkCoord& a, const ChunkCoord& b)
{
  return (a.x == b.x && a.y == b.y && a.z == b.z);
}

bool  operator!=(const ChunkCoord& a, const ChunkCoord& b)
{
  return (!(a == b));
}

bool  operator<(const ChunkCoord& a, const ChunkCoord& b)
{
  if (a.y != b.y) return (a.y < b.y);
  if (a.z != b.z) return (a.z < b.z);
  return (a.x < b.x);
}

static int floorDiv(int a)
{
  int res = a / VOXEL_CHUNK_SIZE;

  if (a < 0 && a % VOXEL_CHUNK_SIZE != 0)
    --res;
  return (res);
}

ChunkCoord  toChunkCoord(int bx, int by, int bz)
{
  ChunkCoord  out;

  out.x = floorDiv(bx);
  out.y = floorDiv(by);
  out.z = floorDiv(bz);
  return (out);
}

static int positiveMod(int a)
{
  int res = a % VOXEL_CHUNK_SIZE;

  return ((res < 0) ? res + VOXEL_CHUNK_SIZE : res);
}

LocalCoord  toLocalCoord(int bx, int by, int bz)
{
  LocalCoord  out;

  out.x = positiveMod(bx);
  out.y = positiveMod(by);
  out.z = positiveMod(bz);
  return (out);
}
