#pragma once

#include "libEngine/utils/macro.h"
#include "libEngine/shared/Vertex.h"
#include <vector>

namespace libEngine
{
class MeshData
{
public:
  std::vector<Vertex>   vertices;
  std::vector<uint32_t> indices;
};
class Mesh
{
public:
  SHARED_PTR_ALIAS(Mesh);

  Mesh();
  virtual void InitBuffer() = 0;
  virtual void Bound()      = 0;
  virtual void Unbound()    = 0;

  virtual void SetData(MeshData data);

  int IndexCount();
  int VertexCount();


  MeshData m_data;
};
}  // namespace libEngine