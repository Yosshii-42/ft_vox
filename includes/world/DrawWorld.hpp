#pragma once

#include "voxel/ChunkData.hpp"
#include "voxel/ChunkMesh.hpp"
#include "voxel/VoxCommon.hpp"
#include "math/Mat4.hpp"
#include "gl/Shader.hpp"

#include <map>
#include <memory>
#include <vector>

class DrawWorld {
private:
  std::map<ChunkCoord, std::unique_ptr<ChunkData>> chunks_;
  std::map<ChunkCoord, std::unique_ptr<ChunkMesh>> meshes_;

public:
  DrawWorld();
  ~DrawWorld();
  DrawWorld(const DrawWorld& other) = delete;
  DrawWorld& operator=(const DrawWorld& other) = delete;

  // chunk
  void              addChunk(const ChunkCoord& coord, const ChunkData& chunk);
  const ChunkData*  getChunk(const ChunkCoord& coord) const;
  
  // mesh
  void              buildMesh(const ChunkCoord& cood);
  void              buildAllMeshes();
  void              addMesh(const ChunkCoord& cood, std::unique_ptr<ChunkMesh> mesh);
  ChunkMesh*        getMesh(const ChunkCoord& coord);
  const ChunkMesh*  getMesh(const ChunkCoord& coord) const;
  
  // block ID
  BlockID           getBlock(int bx, int by, int bz) const;

  // vertices作成
  void  calcVertex(const ChunkCoord& coord, std::vector<Vertex>& vertices) const;
  
  float getTextureLayer(BlockID id) const;
  
  void  addFace(std::vector<Vertex>& vertices, float x, float y, float z, int face, float layer) const;
  void  addFaceNegX(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFacePosX(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFaceNegY(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFacePosY(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFaceNegZ(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFacePosZ(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  
  void  calcGreedyVertex(const ChunkCoord& coord, std::vector<Vertex>& vertices, int face) const;
  
  void  addGreedyFaceNegY(std::vector<Vertex>& vertices, float x, float y, float z,
                          float width, float height, float layer) const;
  void  addGreedyFacePosY(std::vector<Vertex>& vertices, float x, float y, float z,
                          float width, float height, float layer) const;
 
  // 描画
  void  draw(Shader& shader, const Mat4& baseModel,
             const Mat4& view, const Mat4& projection) const;
};
