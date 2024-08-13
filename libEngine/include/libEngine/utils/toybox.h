#pragma once

#include "libEngine/shared/Mesh.h"
#include "libEngine/shared/Vertex.h"

namespace libEngine
{
class Toybox
{
public:
  static MeshData MakeCube(float length, float r, float g, float b);
};
}  // namespace libEngine