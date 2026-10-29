#pragma once

namespace Common
{
  // screen settings
  constexpr unsigned int DEFAULT_WIDTH = 800;
  constexpr unsigned int DEFAULT_HEIGHT = 600;

  // camera settings
  constexpr float CAMERA_DISTANCE = 40.0f;
  constexpr float CAMERA_FOV = 45.0f;
  constexpr float CAMERA_NEAR = 0.1f;
  constexpr float CAMERA_FAR = 100.0f;

  constexpr float VECTOR_EPSILON = 1e-5f;

  constexpr size_t MAX_INSTANCES = 30;

  struct Bounds
  {
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
  };

  enum UVMapping {
    UV_AXIS,
    UV_NORMAL
  };
  
  struct InputFlags
  {
    bool  useTexture;
    bool  addInstance;
    bool  resetInstances;
    UVMapping uvMapping;
  
    InputFlags()
      : useTexture(false),
        addInstance(false),
        resetInstances(false),
        uvMapping(UV_AXIS) {}
  };
}

// constexprとは
// コンパイル時に評価する
// コンパイル時に確定する定数を表す
// 対象は、変数、関数、クラスのコンストラクター