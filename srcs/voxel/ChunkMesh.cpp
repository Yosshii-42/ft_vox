#include"voxel/ChunkMesh.hpp"

#include "voxel/ChunkData.hpp"
#include "voxel/VoxCommon.hpp"

#include<glad/glad.h>
#include<cstddef>

ChunkMesh::~ChunkMesh() {
  glDeleteBuffers(1, &VBO_);
  glDeleteVertexArrays(1, &VAO_);
}

void  ChunkMesh::upload(const std::vector<Vertex>& vertices)
{
  if (VAO_ == 0)
    glGenVertexArrays(1, &VAO_);
  if (VBO_ == 0)
    glGenBuffers(1, &VBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(Vertex),
               vertices.data(),
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

  vertexCount_ = vertices.size();
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
