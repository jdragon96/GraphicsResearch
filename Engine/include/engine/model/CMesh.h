#pragma once

#include "engine/Macro.h"

class CMesh
{
public:
  Mat4  world;
  Mat4  worldInv;
  Mat4  reflection;
  float scale;
  int   useReflection = 0;
  Vec2  dummy1;
};