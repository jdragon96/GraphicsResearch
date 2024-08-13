#include "libEngine/shared/Mesh.h"

namespace libEngine
{
Mesh::Mesh()
{
}
void Mesh::SetData(MeshData data)
{
  m_data = data;
}
int Mesh::IndexCount()
{
  return m_data.indices.size();
}
int Mesh::VertexCount()
{
  return m_data.vertices.size();
}
}  // namespace libEngine