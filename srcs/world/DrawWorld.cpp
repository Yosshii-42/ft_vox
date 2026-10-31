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

void  DrawWorld::calcVertex(const ChunkCoord& coord,
                            std::vector<Vertex>& vertices) const
{
  vertices.clear();

  const ChunkData*  chunk = getChunk(coord);
  if (chunk == nullptr)
    return;

  const int dx[6] = {-1, 1, 0, 0, 0, 0};
  const int dy[6] = {0, 0, -1, 1, 0, 0};
  const int dz[6] = {0, 0, 0, 0, -1, 1};

  for (int y = 0; y < VOXEL_CHUNK_SIZE; ++y)
  {
    for (int z = 0; z < VOXEL_CHUNK_SIZE; ++z)
    {
      for (int x = 0; x < VOXEL_CHUNK_SIZE;  ++x)
      {
        BlockID id = chunk->getBlockID(x, y, z);
        if (id == BLOCK_AIR)
          continue;

        float layer = getTextureLayer(id);

        for (int face = 0; face < 6; ++face)
        {
          // 隣接するチャンクの存在を確認する
          int bx = coord.x * VOXEL_CHUNK_SIZE + x;
          int by = coord.y * VOXEL_CHUNK_SIZE + y;
          int bz = coord.z * VOXEL_CHUNK_SIZE + z;
          BlockID neighbor = getBlock(bx + dx[face], by + dy[face], bz + dz[face]);

          if (neighbor == BLOCK_AIR)
            addFace(vertices, x, y, z, face, layer);
        }
      }
    }
  }
}

float DrawWorld::getTextureLayer(BlockID id) const
{
  switch (id)
  {
  case BLOCK_DIRT:      return 0.0f;
  case BLOCK_STONE:     return 1.0f;
  case BLOCK_BEDROCK:   return 2.0f;
  case BLOCK_SAND:      return 3.0f;
  case BLOCK_SNOW:      return 4.0f;
  case BLOCK_WATER:     return 5.0f;
  case BLOCK_MAGMA:     return 6.0f;
  case BLOCK_CRISTAL:   return 7.0f;
  default:              return 0.0f;
  }
}

void  DrawWorld::addFace(std::vector<Vertex>& vertices,
                         float x, float y, float z, int face, float layer) const
{
  switch(face)
  {
    case 0: addFaceNegX(vertices, x, y, z, layer); break;
    case 1: addFacePosX(vertices, x, y, z, layer); break;
    case 2: addFaceNegY(vertices, x, y, z, layer); break;
    case 3: addFacePosY(vertices, x, y, z, layer); break;
    case 4: addFaceNegZ(vertices, x, y, z, layer); break;
    case 5: addFacePosZ(vertices, x, y, z, layer); break;
  }
}

void  DrawWorld::addFaceNegX(std::vector<Vertex>& vertices,
                             float x, float y, float z, float layer) const
{
  vertices.push_back({x,     y,     z,     0, 0, layer, 0});  // A
  vertices.push_back({x,     y,     z + 1, 1, 0, layer, 0});  // B
  vertices.push_back({x,     y + 1, z + 1, 1, 1, layer, 0});  // C

  vertices.push_back({x,     y + 1, z + 1, 1, 1, layer, 0});  // C
  vertices.push_back({x,     y + 1, z,     0, 1, layer, 0});  // D
  vertices.push_back({x,     y,     z,     0, 0, layer, 0});  // A
}

void  DrawWorld::addFacePosX(std::vector<Vertex>& vertices,
                             float x, float y, float z, float layer) const
{
  vertices.push_back({x + 1, y,     z + 1, 0, 0, layer, 1});
  vertices.push_back({x + 1, y,     z,     1, 0, layer, 1});
  vertices.push_back({x + 1, y + 1, z,     1, 1, layer, 1});

  vertices.push_back({x + 1, y + 1, z,     1, 1, layer, 1});
  vertices.push_back({x + 1, y + 1, z + 1, 0, 1, layer, 1});
  vertices.push_back({x + 1, y,     z + 1, 0, 0, layer, 1});
}

void  DrawWorld::addFaceNegY(std::vector<Vertex>& vertices,
                             float x, float y, float z, float layer) const
{
  vertices.push_back({x,     y,     z,     0, 0, layer, 2});
  vertices.push_back({x + 1, y,     z,     1, 0, layer, 2});
  vertices.push_back({x + 1, y,     z + 1, 1, 1, layer, 2});

  vertices.push_back({x + 1, y,     z + 1, 1, 1, layer, 2});
  vertices.push_back({x,     y,     z + 1, 0, 1, layer, 2});
  vertices.push_back({x,     y,     z,     0, 0, layer, 2});
}

void  DrawWorld::addFacePosY(std::vector<Vertex>& vertices,
                             float x, float y, float z, float layer) const
{
  vertices.push_back({x,     y + 1, z + 1, 0, 0, layer, 3});
  vertices.push_back({x + 1, y + 1, z + 1, 1, 0, layer, 3});
  vertices.push_back({x + 1, y + 1, z,     1, 1, layer, 3});

  vertices.push_back({x + 1, y + 1, z,     1, 1, layer, 3});
  vertices.push_back({x,     y + 1, z,     0, 1, layer, 3});
  vertices.push_back({x,     y + 1, z + 1, 0, 0, layer, 3});
}

void  DrawWorld::addFaceNegZ(std::vector<Vertex>& vertices,
                             float x, float y, float z, float layer) const
{
  vertices.push_back({x + 1, y,     z,     0, 0, layer, 4});
  vertices.push_back({x,     y,     z,     1, 0, layer, 4});
  vertices.push_back({x,     y + 1, z,     1, 1, layer, 4});

  vertices.push_back({x,     y + 1, z,     1, 1, layer, 4});
  vertices.push_back({x + 1, y + 1, z,     0, 1, layer, 4});
  vertices.push_back({x + 1, y,     z,     0, 0, layer, 4});
}

void  DrawWorld::addFacePosZ(std::vector<Vertex>& vertices,
                             float x, float y, float z, float layer) const
{
  vertices.push_back({x,     y,     z + 1, 0, 0, layer, 5});
  vertices.push_back({x + 1, y,     z + 1, 1, 0, layer, 5});
  vertices.push_back({x + 1, y + 1, z + 1, 1, 1, layer, 5});

  vertices.push_back({x + 1, y + 1, z + 1, 1, 1, layer, 5});
  vertices.push_back({x,     y + 1, z + 1, 0, 1, layer, 5});
  vertices.push_back({x,     y,     z + 1, 0, 0, layer, 5});
}
