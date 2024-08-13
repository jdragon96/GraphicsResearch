#include "libEngine/utils/toybox.h"

namespace libEngine
{
MeshData Toybox::MakeCube(float size, float r, float g, float b)
{
  MeshData mesh;

  mesh.vertices.push_back(Vertex(-size, size, -size, r, g, b, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, size, r, g, b, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, size, r, g, b, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(size, size, -size, r, g, b, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, -size, r, g, b, 1.f, 0.f, -1.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, -size, r, g, b, 1.f, 0.f, -1.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, size, r, g, b, 1.f, 0.f, -1.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(-size, -size, size, r, g, b, 1.f, 0.f, -1.f, 0.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(size, -size, -size, r, g, b, 1.f, 0.f, 0.f, -1.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(-size, size, size, r, g, b, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f));

  mesh.vertices.push_back(Vertex(-size, -size, size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(-size, size, -size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(-size, -size, -size, r, g, b, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f));

  mesh.vertices.push_back(Vertex(size, -size, size, r, g, b, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f));
  mesh.vertices.push_back(Vertex(size, -size, -size, r, g, b, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f));
  mesh.vertices.push_back(Vertex(size, size, -size, r, g, b, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f));
  mesh.vertices.push_back(Vertex(size, size, size, r, g, b, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f));

  mesh.indices = {
    0,  1,  2,  0,  2,  3,   // ����
    4,  5,  6,  4,  6,  7,   // �Ʒ���
    8,  9,  10, 8,  10, 11,  // �ո�
    12, 13, 14, 12, 14, 15,  // �޸�
    16, 17, 18, 16, 18, 19,  // ����
    20, 21, 22, 20, 22, 23   // ������
  };

  return mesh;
}
}  // namespace libEngine