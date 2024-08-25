#include <libEngine/opengl/glMesh.h>

namespace libEngine
{
//////////////////////////////////////////////////////////////////// glMesh
glMesh::glMesh() : Mesh()
{
}
glMesh::~glMesh()
{
  if (m_vertexArrayObject)
  {
    glDeleteVertexArrays(1, &m_vertexArrayObject);
  }
  if (m_vertexBufferObject)
  {
    glDeleteBuffers(1, &m_vertexBufferObject);
  }
  if (m_indexBufferObject)
  {
    glDeleteBuffers(1, &m_indexBufferObject);
  }
}
void glMesh::InitBuffer()
{
  glGenVertexArrays(1, &m_vertexArrayObject);
  glGenBuffers(1, &m_vertexBufferObject);
  glGenBuffers(1, &m_indexBufferObject);

  int vertexBufferSize = sizeof(Vertex) * VertexCount();
  int indexBufferSize  = sizeof(uint32_t) * IndexCount();

  glBindVertexArray(m_vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, m_data.vertices.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, m_data.indices.data(), GL_DYNAMIC_DRAW);

  int vtxSize       = sizeof(Vertex);
  int xyzOffset     = 0;
  int rgbOffset     = sizeof(float) * 3;
  int textureOffset = sizeof(float) * 7;
  int normalOffset  = sizeof(float) * 9;

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vtxSize, (void*)xyzOffset);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vtxSize, (void*)rgbOffset);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vtxSize, (void*)textureOffset);

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vtxSize, (void*)normalOffset);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void glMesh::Bound()
{
  glBindVertexArray(m_vertexArrayObject);
}
void glMesh::Unbound()
{
  glBindVertexArray(0);
}
}  // namespace libEngine