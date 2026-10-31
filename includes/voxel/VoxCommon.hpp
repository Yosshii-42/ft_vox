#pragma once

#include <cstdint>
#include <iostream>

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

// チャンク座標。TerrainGenerator の (cx, cy, cz) に対応する
struct ChunkCoord {
  int x{};
  int y{};
  int z{};
};

// チャンク内のローカル座標。各軸 0..VOXEL_CHUNK_SIZE-1
struct LocalCoord {
  int x{};
  int y{};
  int z{};
};

bool  operator==(const ChunkCoord& a, const ChunkCoord& b);
bool  operator!=(const ChunkCoord& a, const ChunkCoord& b);
bool  operator<(const ChunkCoord&a, const ChunkCoord& b);

// ブロック座標 → それを含むチャンクの座標
ChunkCoord  toChunkCoord(int bx, int by, int bz);
// ワールド座標 → それを含むチャンクの座標
LocalCoord  toLocalCoord(int bx, int by, int bz);

// seed
using Seed = std::uint64_t;

// チャンク内の配列添字
inline int  blockIndex(int x, int y, int z)
{
  return (x + VOXEL_CHUNK_SIZE * (z + VOXEL_CHUNK_SIZE * y));
}

inline constexpr int FACE_DX[6] = {-1, 1, 0, 0, 0, 0};
inline constexpr int FACE_DY[6] = {0, 0, -1, 1, 0, 0};
inline constexpr int FACE_DZ[6] = {0, 0, 0, 0, -1, 1};
