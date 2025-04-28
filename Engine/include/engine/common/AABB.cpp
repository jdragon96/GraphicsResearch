#include "engine/common/AABB.h"
#include <algorithm>

bool greaterVertexDataX(VertexData& a, VertexData& b)
{
  return a.x < b.x;
}
bool greaterVertexDataY(VertexData& a, VertexData& b)
{
  return a.y < b.y;
}
bool greaterVertexDataZ(VertexData& a, VertexData& b)
{
  return a.z < b.z;
}

void AABB::Update(MeshBufferBase<VertexData>::SharedPtr mesh)
{
  m_max = Vec3(std::max_element(mesh->m_mesh.vertices.begin(), mesh->m_mesh.vertices.end(), greaterVertexDataX)->x,
               std::max_element(mesh->m_mesh.vertices.begin(), mesh->m_mesh.vertices.end(), greaterVertexDataY)->y,
               std::max_element(mesh->m_mesh.vertices.begin(), mesh->m_mesh.vertices.end(), greaterVertexDataZ)->z);
  m_min = Vec3(std::min_element(mesh->m_mesh.vertices.begin(), mesh->m_mesh.vertices.end(), greaterVertexDataX)->x,
               std::min_element(mesh->m_mesh.vertices.begin(), mesh->m_mesh.vertices.end(), greaterVertexDataY)->y,
               std::min_element(mesh->m_mesh.vertices.begin(), mesh->m_mesh.vertices.end(), greaterVertexDataZ)->z);
}