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
  std::vector<Vertex> vertices_;

  void  addFaceNegX(float x, float y, float z, float layer); // -x面
  void  addFacePosX(float x, float y, float z, float layer); // x面
  void  addFaceNegY(float x, float y, float z, float layer); // -y面
  void  addFacePosY(float x, float y, float z, float layer); // y面
  void  addFaceNegZ(float x, float y, float z, float layer); // -z面
  void  addFacePosZ(float x, float y, float z, float layer); // z面

public:
  ChunkMesh() = default;
  ~ChunkMesh();

  ChunkMesh(const ChunkMesh&) = delete;
  ChunkMesh& operator=(const ChunkMesh&) = delete;

  void  addFace(float x, float y, float z, int face, float layer);
  float getTextureLayer(BlockID id);
  void  build(const ChunkData& chunk);

  void  upload();
  void  draw() const;
};
