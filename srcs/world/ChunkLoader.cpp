#include "world/ChunkLoader.hpp"
#include <utility>
#include <thread>
#include <iostream>
#include <algorithm>

ChunkLoader::ChunkLoader(TerrainGenerator& generator)
  : generator_(generator)
{
  worker_ = std::thread(&ChunkLoader::workerLoop, this);
}

ChunkLoader::~ChunkLoader()
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    stop_ = true;
  }
  cv_.notify_all();
  if (worker_.joinable())
    worker_.join();
}

static long long  distanceSquared(const ChunkCoord& coord, const ChunkCoord& center)
{
  long long dx = static_cast<long long>(coord.x) - center.x;
  long long dy = static_cast<long long>(coord.y) - center.y;
  long long dz = static_cast<long long>(coord.z) - center.z;

  return (dx * dx + dy * dy + dz * dz);
}


void  ChunkLoader::workerLoop()
{
  while (true)
  {
    ChunkCoord  coord;
    {
      std::unique_lock<std::mutex>  lock(mutex_);
      // 生成要求または終了要求があるまで待機する
      cv_.wait(lock, [this] {
        return (!requests_.empty() || stop_);
      });
      if (stop_)
        return;
      
      // 最もカメラに近いチャンクを取り出す
      auto  nearest = std::min_element(
        requests_.begin(),
        requests_.end(),
        [this](const ChunkCoord& a, const ChunkCoord& b) {
          return (distanceSquared(a, currentCenter_) <
                  distanceSquared(b, currentCenter_));
        });

      coord = *nearest;
      requests_.erase(nearest);
    }

    std::unique_ptr<ChunkData>  chunk = std::make_unique<ChunkData>();
    // ロックなし：時間のかかるチャンク生成
    generator_.generateChunk(coord.x, coord.y, coord.z, *chunk);
  
    ChunkResult result{coord, std::move(chunk)};  // ここでchunkはnullptr
    {
      std::lock_guard<std::mutex>  lock(mutex_);
      results_.push(std::move(result));
    }
  }
}

void  ChunkLoader::setCenter(int cx, int cy, int cz)
{
  std::lock_guard<std::mutex> lock(mutex_);
  currentCenter_.x = cx;
  currentCenter_.y = cy;
  currentCenter_.z = cz;
}

void  ChunkLoader::requestChunk(const ChunkCoord& coord)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    // 生成要求をキューに追加する
    requests_.push_back(coord);
  }
  // 待機中のワーカーに仕事があることを知らせる
  cv_.notify_one();
}

bool  ChunkLoader::pollResult(ChunkResult& result)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);

    if (results_.empty())
      return (false);

    result = std::move(results_.front());
    results_.pop();
    return (true);
  }
}

