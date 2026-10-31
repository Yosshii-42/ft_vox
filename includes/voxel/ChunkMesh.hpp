// ブロックの面を頂点データに変換する
#pragma once

#include "voxel/ChunkData.hpp"
#include "voxel/VoxCommon.hpp"

#include<iostream>
#include<vector>

struct Vertex {
  float x, y, z;
  float u, v;
  float layer;  // テクスチャレイヤー番号
  float face;   // 面の方向（0〜5）
};

class ChunkMesh {
private:
  unsigned int  VAO_ = 0;
  unsigned int  VBO_ = 0;
  size_t        vertexCount_ = 0;

public:
  ChunkMesh() = default;
  ~ChunkMesh();

  ChunkMesh(const ChunkMesh&) = delete;
  ChunkMesh& operator=(const ChunkMesh&) = delete;

  void  upload(const std::vector<Vertex>& vertices);
  void  draw() const;
};
