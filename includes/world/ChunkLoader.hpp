#pragma once

#include "voxel/VoxCommon.hpp"
#include "voxel/ChunkData.hpp"
#include "world/TerrainGenerator.hpp"
#include "math/Vec3.hpp"
// #include <condition_variable>
// #include <mutex>
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
  Vec3                    currentForward_{0.0f, 0.0f, -1.0f};
  std::queue<ChunkResult> results_;   // ワーカー書き込み、メイン読み出し
  bool                    isFirstRequest_ = true;

  mutable std::mutex      mutex_; // const 関数内でもロックできる
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
  
  void                    requestChunk(const ChunkCoord& coord);
  bool                    pollResult(ChunkResult& result);
  std::vector<ChunkCoord> setCenter(const ChunkCoord& center, int radius);
  void                    setForward(const Vec3& forward);
  float                   calcPriority(const ChunkCoord& coord) const;
  float                   calcDirectionDot(const ChunkCoord& coord) const;
  size_t                  getRequestsCount() const;
  size_t                  getResultsCount() const;
};
