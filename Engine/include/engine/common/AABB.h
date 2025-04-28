#pragma once

#include "engine/Macro.h"
#include "engine/model/VertexData.h"
#include "engine/interface/MeshBufferBase.h"

bool greaterVertexDataX(VertexData& a, VertexData& b);
bool greaterVertexDataY(VertexData& a, VertexData& b);
bool greaterVertexDataZ(VertexData& a, VertexData& b);

class AABB
{
public:
  SHARED_PTR(AABB);

  void Update(MeshBufferBase<VertexData>::SharedPtr mesh);

  Vec3 m_min;
  Vec3 m_max;
};