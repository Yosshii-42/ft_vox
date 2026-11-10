#include "world/ChunkLoader.hpp"
#include <utility>
#include <thread>

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
  worker_.join();
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
      
      coord = requests_.front();
      requests_.pop();
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

void  ChunkLoader::requestChunk(const ChunkCoord& coord)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    // 生成要求をキューに追加する
    requests_.push(coord);
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

