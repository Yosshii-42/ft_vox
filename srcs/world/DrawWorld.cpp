#include "world/DrawWorld.hpp"

#include "voxel/VoxCommon.hpp"

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

// upload()はメインスレッドで呼び出す
void  DrawWorld::buildMesh(const ChunkCoord& coord)
{
  if (getChunk(coord) == nullptr)
    return;

  std::vector<Vertex> vertices;

  #ifdef DEBUG_NORMAL_MESH
    calcVertex(coord, vertices);
  #else
    for (int face = 0; face < 6; ++face)
    {
      if (face == 0 || face == 1)
        calcGreedyVertexUVx(coord, vertices, face);
      else if (face == 2 || face == 3)
        calcGreedyVertexUVy(coord, vertices, face);
      else
        calcGreedyVertexUVz(coord, vertices, face);
    }
  #endif

  ChunkMesh*  mesh = getMesh(coord);
  if (mesh != nullptr)
  {
    mesh->upload(vertices); // ->GPU
    return;
  }

  auto  newMesh = std::make_unique<ChunkMesh>();
  newMesh->upload(vertices);  // ->GPU
  addMesh(coord, std::move(newMesh));
}

void  DrawWorld::buildAllMeshes()
{
  for (const auto& chunk : chunks_)
    buildMesh(chunk.first); // チャンク座標
}

// ChunkMeshはVAO,VBOなどのGPUリソースを所有しているのでコピーできない
void  DrawWorld::addMesh(const ChunkCoord& coord, std::unique_ptr<ChunkMesh> mesh)
{
  if (meshes_.find(coord) != meshes_.end())
    return ;

  meshes_[coord] = std::move(mesh); // ChunkMeshの所有権をworldに移す
}

ChunkMesh*  DrawWorld::getMesh(const ChunkCoord& coord)
{
  auto it = meshes_.find(coord);

  if (it == meshes_.end())
    return nullptr;

  return it->second.get();
}

const ChunkMesh*  DrawWorld::getMesh(const ChunkCoord& coord) const
{
  auto it = meshes_.find(coord);

  if (it == meshes_.end())
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

void  DrawWorld::calcGreedyVertexUVx(const ChunkCoord& coord,
                                  std::vector<Vertex>& vertices, int face) const
{
  const ChunkData*  chunk = getChunk(coord);
  if (chunk == nullptr)
    return;

  const int size = VOXEL_CHUNK_SIZE;

  for (int x = 0; x < size; ++x)
  {
    BlockID mask[VOXEL_CHUNK_SIZE][VOXEL_CHUNK_SIZE];

    // x面 見えているブロックを記録
    for (int y = 0; y < size; ++y)
    {
      for (int z = 0; z < size; ++z)
      {
        mask[y][z] = BLOCK_AIR;

        BlockID id = chunk->getBlockID(x, y, z);
        if (id == BLOCK_AIR)
          continue;

        int worldX = coord.x * size + x;
        int worldY = coord.y * size + y;
        int worldZ = coord.z * size + z;

        if (getBlock(worldX + FACE_DX[face],
                     worldY + FACE_DY[face],
                     worldZ + FACE_DZ[face]) == BLOCK_AIR)
          mask[y][z] = id;
      }
    }

    // 同じブロック面を統合
    for (int y = 0; y < size; ++y)
    {
      for (int z = 0; z < size; ++z)
      {
        BlockID id = mask[y][z];
        if (id == BLOCK_AIR)
          continue;

        // 横方向の幅を得る
        int width = 1;
        while (z + width < size && mask[y][z + width] == id)
          ++width;

        // 縦方向の長さを得る
        int height = 1;
        bool  canExpand = true;
        while (y + height < size && canExpand)
        {
          for (int dz = 0; dz < width; ++dz)
          {
            if (mask[y + height][z + dz] != id)
            {
              canExpand = false;  
              break;
            }
          }
          if (canExpand)
            ++height;
        }

        // 結合した面の頂点を生成
        float gx = static_cast<float>(x);
        float gy = static_cast<float>(y);
        float gz = static_cast<float>(z);
        float gw = static_cast<float>(width);
        float gh = static_cast<float>(height);

        if (face == 0)
          addGreedyFaceNegX(vertices, gx, gy, gz, gw, gh, getTextureLayer(id));
        else if (face == 1)
          addGreedyFacePosX(vertices, gx, gy, gz, gw, gh, getTextureLayer(id));

        // マスク消去
        for (int dy = 0; dy < height; ++dy)
        {
          for (int dz = 0; dz < width; ++dz)
            mask[y + dy][z + dz] = BLOCK_AIR;
        }
      }
    }
  }
}

void  DrawWorld::calcGreedyVertexUVy(const ChunkCoord& coord,
                                  std::vector<Vertex>& vertices, int face) const
{
  const ChunkData*  chunk = getChunk(coord);
  if (chunk == nullptr)
    return;

  const int size = VOXEL_CHUNK_SIZE;

  for (int y = 0; y < size; ++y)
  {
    BlockID mask[VOXEL_CHUNK_SIZE][VOXEL_CHUNK_SIZE];

    // 上面(y面)見えているブロックを記録
    for (int z = 0; z < size; ++z)
    {
      for (int x = 0; x < size; ++x)
      {
        mask[z][x] = BLOCK_AIR;

        BlockID id = chunk->getBlockID(x, y, z);
        if (id == BLOCK_AIR)
          continue;

        int worldX = coord.x * size + x;
        int worldY = coord.y * size + y;
        int worldZ = coord.z * size + z;

        if (getBlock(worldX + FACE_DX[face],
                     worldY + FACE_DY[face],
                     worldZ + FACE_DZ[face]) == BLOCK_AIR)
          mask[z][x] = id;
      }
    }

    // 同じブロック面を統合
    for (int z = 0; z < size; ++z)
    {
      for (int x = 0; x < size; ++x)
      {
        BlockID id = mask[z][x];
        if (id == BLOCK_AIR)
          continue;

        // 横方向の幅を得る
        int width = 1;
        while (x + width < size && mask[z][x + width] == id)
          ++width;

        // 縦方向の長さを得る
        int height = 1;
        bool  canExpand = true;
        while (z + height < size && canExpand)
        {
          for (int dx = 0; dx < width; ++dx)
          {
            if (mask[z + height][x + dx] != id)
            {
              canExpand = false;  
              break;
            }
          }
          if (canExpand)
            ++height;
        }

        // 結合した面の頂点を生成
        float gx = static_cast<float>(x);
        float gy = static_cast<float>(y);
        float gz = static_cast<float>(z);
        float gw = static_cast<float>(width);
        float gh = static_cast<float>(height);

        if (face == 2)
          addGreedyFaceNegY(vertices, gx, gy, gz, gw, gh, getTextureLayer(id));
        else if (face == 3)
          addGreedyFacePosY(vertices, gx, gy, gz, gw, gh, getTextureLayer(id));

        // マスク消去
        for (int dz = 0; dz < height; ++dz)
        {
          for (int dx = 0; dx < width; ++dx)
            mask[z + dz][x + dx] = BLOCK_AIR;
        }
      }
    }
  }
}


void  DrawWorld::calcGreedyVertexUVz(const ChunkCoord& coord,
                                  std::vector<Vertex>& vertices, int face) const
{
  const ChunkData*  chunk = getChunk(coord);
  if (chunk == nullptr)
    return;

  const int size = VOXEL_CHUNK_SIZE;

  for (int z = 0; z < size; ++z)
  {
    BlockID mask[VOXEL_CHUNK_SIZE][VOXEL_CHUNK_SIZE];

    // x面 見えているブロックを記録
    for (int y = 0; y < size; ++y)
    {
      for (int x = 0; x < size; ++x)
      {
        mask[y][x] = BLOCK_AIR;

        BlockID id = chunk->getBlockID(x, y, z);
        if (id == BLOCK_AIR)
          continue;

        int worldX = coord.x * size + x;
        int worldY = coord.y * size + y;
        int worldZ = coord.z * size + z;

        if (getBlock(worldX + FACE_DX[face],
                     worldY + FACE_DY[face],
                     worldZ + FACE_DZ[face]) == BLOCK_AIR)
          mask[y][x] = id;
      }
    }

    // 同じブロック面を統合
    for (int y = 0; y < size; ++y)
    {
      for (int x = 0; x < size; ++x)
      {
        BlockID id = mask[y][x];
        if (id == BLOCK_AIR)
          continue;

        // 横方向の幅を得る
        int width = 1;
        while (x + width < size && mask[y][x + width] == id)
          ++width;

        // 縦方向の長さを得る
        int height = 1;
        bool  canExpand = true;
        while (y + height < size && canExpand)
        {
          for (int dx = 0; dx < width; ++dx)
          {
            if (mask[y + height][x + dx] != id)
            {
              canExpand = false;  
              break;
            }
          }
          if (canExpand)
            ++height;
        }

        // 結合した面の頂点を生成
        float gx = static_cast<float>(x);
        float gy = static_cast<float>(y);
        float gz = static_cast<float>(z);
        float gw = static_cast<float>(width);
        float gh = static_cast<float>(height);

        if (face == 4)
          addGreedyFaceNegZ(vertices, gx, gy, gz, gw, gh, getTextureLayer(id));
        else if (face == 5)
          addGreedyFacePosZ(vertices, gx, gy, gz, gw, gh, getTextureLayer(id));

        // マスク消去
        for (int dy = 0; dy < height; ++dy)
        {
          for (int dx = 0; dx < width; ++dx)
            mask[y + dy][x + dx] = BLOCK_AIR;
        }
      }
    }
  }
}

void  DrawWorld::addGreedyFaceNegX(std::vector<Vertex>& vertices,
                                   float x, float y, float z,
                                   float w, float h, float layer) const
{
  vertices.push_back({x,     y,     z,     0, 0, layer, 0});  // A
  vertices.push_back({x,     y,     z + w, w, 0, layer, 0});  // B
  vertices.push_back({x,     y + h, z + w, w, h, layer, 0});  // C

  vertices.push_back({x,     y + h, z + w, w, h, layer, 0});  // C
  vertices.push_back({x,     y + h, z,     0, h, layer, 0});  // D
  vertices.push_back({x,     y,     z,     0, 0, layer, 0});  // A
}

void  DrawWorld::addGreedyFacePosX(std::vector<Vertex>& vertices,
                                   float x, float y, float z,
                                   float w, float h, float layer) const
{
  vertices.push_back({x + 1, y,     z + w, 0, 0, layer, 1});
  vertices.push_back({x + 1, y,     z,     w, 0, layer, 1});
  vertices.push_back({x + 1, y + h, z,     w, h, layer, 1});

  vertices.push_back({x + 1, y + h, z,     w, h, layer, 1});
  vertices.push_back({x + 1, y + h, z + w, 0, h, layer, 1});
  vertices.push_back({x + 1, y,     z + w, 0, 0, layer, 1});
}

void  DrawWorld::addGreedyFaceNegY(std::vector<Vertex>& vertices,
                                   float x, float y, float z,
                                   float w, float h, float layer) const
{
  vertices.push_back({x,     y,     z,     0, 0, layer, 2});
  vertices.push_back({x + w, y,     z,     w, 0, layer, 2});
  vertices.push_back({x + w, y,     z + h, w, h, layer, 2});

  vertices.push_back({x + w, y,     z + h, w, h, layer, 2});
  vertices.push_back({x,     y,     z + h, 0, h, layer, 2});
  vertices.push_back({x,     y,     z,     0, 0, layer, 2});
}

void  DrawWorld::addGreedyFacePosY(std::vector<Vertex>& vertices,
                                   float x, float y, float z,
                                   float w, float h, float layer) const
{
  vertices.push_back({x,     y + 1, z + h, 0, 0, layer, 3});
  vertices.push_back({x + w, y + 1, z + h, w, 0, layer, 3});
  vertices.push_back({x + w, y + 1, z,     w, h, layer, 3});

  vertices.push_back({x + w, y + 1, z,     w, h, layer, 3});
  vertices.push_back({x,     y + 1, z,     0, h, layer, 3});
  vertices.push_back({x,     y + 1, z + h, 0, 0, layer, 3});
}

void  DrawWorld::addGreedyFaceNegZ(std::vector<Vertex>& vertices,
                                   float x, float y, float z,
                                   float w, float h, float layer) const{
  vertices.push_back({x + w, y,     z,     0, 0, layer, 4});
  vertices.push_back({x,     y,     z,     w, 0, layer, 4});
  vertices.push_back({x,     y + h, z,     w, h, layer, 4});

  vertices.push_back({x,     y + h, z,     w, h, layer, 4});
  vertices.push_back({x + w, y + h, z,     0, h, layer, 4});
  vertices.push_back({x + w, y,     z,     0, 0, layer, 4});
}

void  DrawWorld::addGreedyFacePosZ(std::vector<Vertex>& vertices,
                                   float x, float y, float z,
                                   float w, float h, float layer) const{
  vertices.push_back({x,     y,     z + 1, 0, 0, layer, 5});
  vertices.push_back({x + w, y,     z + 1, w, 0, layer, 5});
  vertices.push_back({x + w, y + h, z + 1, w, h, layer, 5});

  vertices.push_back({x + w, y + h, z + 1, w, h, layer, 5});
  vertices.push_back({x,     y + h, z + 1, 0, h, layer, 5});
  vertices.push_back({x,     y,     z + 1, 0, 0, layer, 5});
}

// 描画
void  DrawWorld::draw(Shader& shader, const Mat4& baseModel,
                      const Mat4& view, const Mat4& projection) const
{
  for (const auto& entry : meshes_)
  {
    const ChunkCoord&  coord = entry.first;
    const ChunkMesh*   mesh = entry.second.get();
    if (mesh == nullptr)
      continue;

    Mat4  model = baseModel * Mat4::translate(coord.x * VOXEL_CHUNK_SIZE,
                                              coord.y * VOXEL_CHUNK_SIZE,
                                              coord.z * VOXEL_CHUNK_SIZE);
    shader.setMatrices(model, view, projection);
    mesh->draw();
  }
}

void  DrawWorld::calcVertex(const ChunkCoord& coord,
                            std::vector<Vertex>& vertices) const
{
  vertices.clear();

  const ChunkData*  chunk = getChunk(coord);
  if (chunk == nullptr)
    return;

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
          BlockID neighbor = getBlock(bx + FACE_DX[face],
                                      by + FACE_DY[face],
                                      bz + FACE_DZ[face]);

          if (neighbor == BLOCK_AIR)
            addFace(vertices, x, y, z, face, layer);
        }
      }
    }
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
