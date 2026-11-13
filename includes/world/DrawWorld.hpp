#pragma once

#include "voxel/ChunkData.hpp"
#include "voxel/ChunkMesh.hpp"
#include "voxel/VoxCommon.hpp"
#include "math/Mat4.hpp"
#include "gl/Shader.hpp"
#include "gl/Camera.hpp"
#include "world/TerrainGenerator.hpp"
#include "world/ChunkLoader.hpp"
#include <map>
#include <set>
#include <memory>
#include <vector>

struct RenderStats {
  size_t  vertices = 0;
  size_t  chunks = 0;
};

class DrawWorld {
private:
  std::map<ChunkCoord, std::unique_ptr<ChunkData>>  chunks_;
  std::map<ChunkCoord, std::unique_ptr<ChunkMesh>>  meshes_;
  std::set<ChunkCoord>  dirtyChunks_;
  std::set<ChunkCoord>  pendingChunks_;
  ChunkLoader           loader_;

  static constexpr int  RENDER_RADIUS = 8;
  int renderRadius_ = RENDER_RADIUS;  

public:
  static constexpr int  UNLOAD_RADIUS = RENDER_RADIUS + 1;
  static constexpr int  WORLD_HEIGHT = 256;
  static constexpr int  WORLD_CHUNK_HEIGHT = WORLD_HEIGHT / VOXEL_CHUNK_SIZE;


  DrawWorld(TerrainGenerator& generator);
  ~DrawWorld();
  DrawWorld(const DrawWorld& other) = delete;
  DrawWorld& operator=(const DrawWorld& other) = delete;

  // radius
  void              setRenderRadius(int radius);
  int               getRenderRadius() const;

  // block
  void              setBlock(int bx, int by, int bz, BlockID id);

  // dirtyChunks
  void              markChunkDirty(const ChunkCoord& coord);
  void              updateDirtyChunks(const Camera& camera);

  // chunk
  void              addChunk(const ChunkCoord& coord, std::unique_ptr<ChunkData> chunk);
  const ChunkData*  getChunk(const ChunkCoord& coord) const;
  ChunkData*        getMutableChunk(const ChunkCoord& coord);
    // ->GPU
  void  loadAround(int centerCx, int centerCy, int centerCz);
  void  collectLoadedChunks();
  void  unloadFarChunks(int centerCx, int centerCz);
  

  // mesh
  void              buildMesh(const ChunkCoord& cood);
  void              buildAllMeshes();
  void              addMesh(const ChunkCoord& cood, std::unique_ptr<ChunkMesh> mesh);
  ChunkMesh*        getMesh(const ChunkCoord& coord);
  const ChunkMesh*  getMesh(const ChunkCoord& coord) const;
  
  // block ID
  BlockID           getBlock(int bx, int by, int bz) const;
  // get Layer
  float getTextureLayer(BlockID id, int face) const;

  // vertices作成
  void  calcGreedyVertexUVx(const ChunkCoord& coord, std::vector<Vertex>& vertices, int face) const;
  void  calcGreedyVertexUVy(const ChunkCoord& coord, std::vector<Vertex>& vertices, int face) const;
  void  calcGreedyVertexUVz(const ChunkCoord& coord, std::vector<Vertex>& vertices, int face) const;
  
  void  addGreedyFaceNegX(std::vector<Vertex>& vertices, float x, float y, float z,
    float w, float h, float layer) const;
  void  addGreedyFacePosX(std::vector<Vertex>& vertices, float x, float y, float z,
    float w, float h, float layer) const;
  void  addGreedyFaceNegY(std::vector<Vertex>& vertices, float x, float y, float z,
    float w, float h, float layer) const;
  void  addGreedyFacePosY(std::vector<Vertex>& vertices, float x, float y, float z,
    float w, float h, float layer) const;
  void  addGreedyFaceNegZ(std::vector<Vertex>& vertices, float x, float y, float z,
    float w, float h, float layer) const;
  void  addGreedyFacePosZ(std::vector<Vertex>& vertices, float x, float y, float z,
    float w, float h, float layer) const;

  // 描画
  RenderStats  draw(Shader& shader, const Mat4& baseModel,
                    const Mat4& view, const Mat4& projection,
                    int centerCx, int centerCz) const;

  void  testChunkBoundary(TerrainGenerator& generator);

  // 旧Vertices作成
  void  calcVertex(const ChunkCoord& coord, std::vector<Vertex>& vertices, int face) const;
  void  addFace(std::vector<Vertex>& vertices, float x, float y, float z, int face, float layer) const;
  void  addFaceNegX(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFacePosX(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFaceNegY(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFacePosY(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFaceNegZ(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
  void  addFacePosZ(std::vector<Vertex>& vertices, float x, float y, float z, float layer) const;
};
