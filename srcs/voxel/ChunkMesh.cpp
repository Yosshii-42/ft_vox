#include"voxel/ChunkMesh.hpp"

#include "voxel/ChunkData.hpp"
#include "voxel/VoxCommon.hpp"

#include<glad/glad.h>
#include<cstddef>

ChunkMesh::~ChunkMesh() {
  glDeleteBuffers(1, &VBO_);
  glDeleteVertexArrays(1, &VAO_);
}

void  ChunkMesh::addFace(float x, float y, float z, int face, float layer)
{
  switch(face)
  {
    case 0: addFaceNegX(x, y, z, layer); break;
    case 1: addFacePosX(x, y, z, layer); break;
    case 2: addFaceNegY(x, y, z, layer); break;
    case 3: addFacePosY(x, y, z, layer); break;
    case 4: addFaceNegZ(x, y, z, layer); break;
    case 5: addFacePosZ(x, y, z, layer); break;
  }
}

// 隣接面を判定し、必要な面の頂点を生成
void  ChunkMesh::build(const ChunkData& chunk)
{
  const int dx[6] = {-1, 1, 0, 0, 0, 0};
  const int dy[6] = {0, 0, -1, 1, 0, 0};
  const int dz[6] = {0, 0, 0, 0, -1, 1};

  vertices_.clear();

  for (int y = 0; y < VOXEL_CHUNK_SIZE; ++y)
  {
    for (int z = 0; z < VOXEL_CHUNK_SIZE; ++z)
    {
      for (int x = 0; x < VOXEL_CHUNK_SIZE; ++x)
      {
        BlockID id = chunk.getBlockID(x ,y, z);
        if (id == BLOCK_AIR)
          continue;
        
        float layer = getTextureLayer(id);
        for (int face = 0; face < 6; ++face)
        {
          // チャンク内の隣接ブロックがAIRだったら、面を作成する
          if (chunk.getBlockID(x + dx[face],
                               y + dy[face],
                               z + dz[face]) == BLOCK_AIR)
            addFace(x, y, z, face, layer);
        }
      }
    }
  }
}

void  ChunkMesh::upload()
{
  if (VAO_ == 0)
    glGenVertexArrays(1, &VAO_);
  if (VBO_ == 0)
    glGenBuffers(1, &VBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  glBufferData(GL_ARRAY_BUFFER,
               vertices_.size() * sizeof(Vertex),
               vertices_.data(),
               GL_STATIC_DRAW);
  
  // position location = 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, x)));
  glEnableVertexAttribArray(0);

  // uv location = 1
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, u)));
  glEnableVertexAttribArray(1);

  // layer location = 2
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, layer)));
  glEnableVertexAttribArray(2);

  // face location = 3
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void*>(offsetof(Vertex, face)));
  glEnableVertexAttribArray(3);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  vertexCount_ = vertices_.size();
}

void  ChunkMesh::draw() const
{
  if (vertexCount_ == 0)
    return ;
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0,
               static_cast<GLsizei>(vertexCount_));
  glBindVertexArray(0);
}

void  ChunkMesh::addFaceNegX(float x, float y, float z, float layer)
{
  vertices_.push_back({x,     y,     z,     0, 0, layer, 0});  // A
  vertices_.push_back({x,     y,     z + 1, 1, 0, layer, 0});  // B
  vertices_.push_back({x,     y + 1, z + 1, 1, 1, layer, 0});  // C

  vertices_.push_back({x,     y + 1, z + 1, 1, 1, layer, 0});  // C
  vertices_.push_back({x,     y + 1, z,     0, 1, layer, 0});  // D
  vertices_.push_back({x,     y,     z,     0, 0, layer, 0});  // A
}

void  ChunkMesh::addFacePosX(float x, float y, float z, float layer)
{
  vertices_.push_back({x + 1, y,     z + 1, 0, 0, layer, 1});
  vertices_.push_back({x + 1, y,     z,     1, 0, layer, 1});
  vertices_.push_back({x + 1, y + 1, z,     1, 1, layer, 1});

  vertices_.push_back({x + 1, y + 1, z,     1, 1, layer, 1});
  vertices_.push_back({x + 1, y + 1, z + 1, 0, 1, layer, 1});
  vertices_.push_back({x + 1, y,     z + 1, 0, 0, layer, 1});
}

void  ChunkMesh::addFaceNegY(float x, float y, float z, float layer)
{
  vertices_.push_back({x,     y,     z,     0, 0, layer, 2});
  vertices_.push_back({x + 1, y,     z,     1, 0, layer, 2});
  vertices_.push_back({x + 1, y,     z + 1, 1, 1, layer, 2});

  vertices_.push_back({x + 1, y,     z + 1, 1, 1, layer, 2});
  vertices_.push_back({x,     y,     z + 1, 0, 1, layer, 2});
  vertices_.push_back({x,     y,     z,     0, 0, layer, 2});
}

void  ChunkMesh::addFacePosY(float x, float y, float z, float layer)
{
  vertices_.push_back({x,     y + 1, z + 1, 0, 0, layer, 3});
  vertices_.push_back({x + 1, y + 1, z + 1, 1, 0, layer, 3});
  vertices_.push_back({x + 1, y + 1, z,     1, 1, layer, 3});

  vertices_.push_back({x + 1, y + 1, z,     1, 1, layer, 3});
  vertices_.push_back({x,     y + 1, z,     0, 1, layer, 3});
  vertices_.push_back({x,     y + 1, z + 1, 0, 0, layer, 3});
}

void  ChunkMesh::addFaceNegZ(float x, float y, float z, float layer)
{
  vertices_.push_back({x + 1, y,     z,     0, 0, layer, 4});
  vertices_.push_back({x,     y,     z,     1, 0, layer, 4});
  vertices_.push_back({x,     y + 1, z,     1, 1, layer, 4});

  vertices_.push_back({x,     y + 1, z,     1, 1, layer, 4});
  vertices_.push_back({x + 1, y + 1, z,     0, 1, layer, 4});
  vertices_.push_back({x + 1, y,     z,     0, 0, layer, 4});
}

void  ChunkMesh::addFacePosZ(float x, float y, float z, float layer)
{
  vertices_.push_back({x,     y,     z + 1, 0, 0, layer, 5});
  vertices_.push_back({x + 1, y,     z + 1, 1, 0, layer, 5});
  vertices_.push_back({x + 1, y + 1, z + 1, 1, 1, layer, 5});

  vertices_.push_back({x + 1, y + 1, z + 1, 1, 1, layer, 5});
  vertices_.push_back({x,     y + 1, z + 1, 0, 1, layer, 5});
  vertices_.push_back({x,     y,     z + 1, 0, 0, layer, 5});
}

float ChunkMesh::getTextureLayer(BlockID id)
{
  switch (id)
  {
    case BLOCK_DIRT:    return 0.0f;
    case BLOCK_STONE:   return 1.0f;
    case BLOCK_BEDROCK: return 2.0f;
    case BLOCK_SAND:    return 3.0f;
    case BLOCK_SNOW:    return 4.0f;
    case BLOCK_WATER:   return 5.0f;
    case BLOCK_MAGMA:   return 6.0f;
    case BLOCK_CRISTAL: return 7.0f;
    default:            return 0.0f;
  }
}
