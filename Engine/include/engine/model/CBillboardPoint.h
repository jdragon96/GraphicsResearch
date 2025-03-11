#pragma once

#include "engine/Macro.h"

class CBillboardPoint
{
public:
  Mat4  viewGeom;
  Mat4  projGeom;
  Vec3  eyeWorldPos;
  float dummy;
  Vec3  eyeUpVector;
  float dummy1;
};