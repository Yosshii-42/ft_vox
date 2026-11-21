#include "world/ChunkLoader.hpp"
#include "voxel/VoxCommon.hpp"
#include <utility>
#include <thread>
#include <iostream>
#include <algorithm>
#include <vector>

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

      // 初回だけ5ms待機する
      if (isFirstRequest_)
      {
        isFirstRequest_ = false;
        auto  deadline = std::chrono::steady_clock::now() +
                         std::chrono::milliseconds(5);

        while (!stop_ && std::chrono::steady_clock::now() < deadline)
          cv_.wait_until(lock, deadline);
      }
      if (stop_)
        return;
      
      // 描画の優先度が高いチャンクを取り出す
      auto  nearest = std::min_element(
        requests_.begin(),
        requests_.end(),
        [this](const ChunkCoord& a, const ChunkCoord& b) {
          return (calcPriority(a) < calcPriority(b));
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

std::vector<ChunkCoord> ChunkLoader::setCenter(const ChunkCoord& center, int radius)
{
  std::lock_guard<std::mutex> lock(mutex_);
  
  currentCenter_ = center;
  std::vector<ChunkCoord> removeChunks;

  auto rmvIter = std::remove_if(
    requests_.begin(),
    requests_.end(),
    [center, radius, &removeChunks](const ChunkCoord& coord)
    {
      if (isOutsideRadius(coord.x, coord.z, center, radius))
      {
        removeChunks.push_back(coord);
        return true;
      }
      return false;
    }
  );
  requests_.erase(rmvIter, requests_.end());
  
  return removeChunks;
}

void  ChunkLoader::setForward(const Vec3& forward)
{
  std::lock_guard<std::mutex> lock(mutex_);
  currentForward_ = forward;
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

float ChunkLoader::calcPriority(const ChunkCoord& coord) const
{
  long long distSq = distanceSquared(coord, currentCenter_);
  if (distSq == 0)
    return 0.0f;

  float dot = calcDirectionDot(coord);
  float weight = 1.0f - dot * 0.8f; // 0.8は優先度の係数
  float priority = distSq * weight;

  return priority;
}

float ChunkLoader::calcDirectionDot(const ChunkCoord& coord) const
{
  long long distSq = distanceSquared(coord, currentCenter_);
  if (distSq == 0)
    return 0.0f;

  Vec3  toChunk = Vec3(coord.x - currentCenter_.x,
                       coord.y - currentCenter_.y,
                       coord.z - currentCenter_.z).normalize();
  return (currentForward_.dot(toChunk));  
}
