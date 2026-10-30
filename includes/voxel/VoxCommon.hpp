#pragma once

#include <cstdint>

// chunk size
constexpr int VOXEL_CHUNK_SIZE = 16;
constexpr int VOXEL_CHUNK_VOLUME =
                VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE * VOXEL_CHUNK_SIZE;

// Block type
using BlockID = std::uint8_t;

constexpr BlockID BLOCK_AIR     = 0;
constexpr BlockID BLOCK_DIRT    = 1;
constexpr BlockID BLOCK_STONE   = 2;
constexpr BlockID BLOCK_BEDROCK = 3;

constexpr BlockID BLOCK_SAND    = 4;
constexpr BlockID BLOCK_SNOW    = 5;
constexpr BlockID BLOCK_WATER   = 6;
constexpr BlockID BLOCK_MAGMA   = 7;
constexpr BlockID BLOCK_CRISTAL = 8;

constexpr BlockID BLOCK_ERROR = 255;

// seed
using Seed = std::uint64_t;

// チャンク内の配列添字
inline int  blockIndex(int x, int y, int z)
{
  return (x + VOXEL_CHUNK_SIZE * (z + VOXEL_CHUNK_SIZE * y));
}
