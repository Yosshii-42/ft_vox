#pragma once

#include "voxel/VoxCommon.hpp"
#include "voxel/ChunkData.hpp"
#include "world/TerrainGenerator.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

class ChunkLoader {
public:
  struct ChunkResult {
    ChunkCoord                  coord;
    std::unique_ptr<ChunkData>  chunk;
  };

private:
  std::vector<ChunkCoord> requests_;  // メイン書き込み、ワーカー読み出し
  ChunkCoord              currentCenter_{0, 0, 0};
  std::queue<ChunkResult> results_;   // ワーカー書き込み、メイン読み出し

  std::mutex              mutex_;
  std::condition_variable cv_;
  std::thread             worker_;
  const TerrainGenerator& generator_;

  bool  stop_ = false;

  void  workerLoop();
  
  public:
  explicit ChunkLoader(TerrainGenerator& generator);
  ~ChunkLoader();
  ChunkLoader(const ChunkLoader& other) = delete;
  ChunkLoader& operator=(const ChunkLoader& other) = delete;
  
  void  requestChunk(const ChunkCoord& coord);
  bool  pollResult(ChunkResult& result);
  void  setCenter(int cx, int cy, int cz);
};
