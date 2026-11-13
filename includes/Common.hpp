#pragma once

#include <cstddef>
#include "math/Vec3.hpp"

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

  // light settings
  inline const Vec3 AMBIENT_LIGHT(1.0f, 1.0f, 1.0f);
  constexpr float   AMBIENT_STRENGTH = 0.75f;
  inline const Vec3 LIGHT_DIRECTION(-1.0f, 1.0f, 0.0f);
  constexpr float   SUN_LIGHT_STRENGTH = 0.25;

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