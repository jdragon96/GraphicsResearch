#include <libEngine/opengl/glMeshBuffer.h>

namespace libEngine
{
//////////////////////////////////////////////////////////////////// MeshBuffer
glMeshBuffer::glMeshBuffer()
{
}
glMeshBuffer::~glMeshBuffer()
{
}
void glMeshBuffer::SetTextureBuffer(unsigned int textureBuf)
{
}
void glMeshBuffer::SetMesh(std::vector<MeshData> mesh)
{
  m_meshData.clear();
  for (auto& m : mesh)
  {
    auto meshBuf = glMesh::MakeShared();
    meshBuf->SetData(m);
    m_meshData.push_back(meshBuf);
  }
}
void glMeshBuffer::Initialize()
{
  for (auto& mesh : m_meshData)
  {
    mesh->InitBuffer();
  }
}
void glMeshBuffer::Render()
{
  m_mainShader->Bound();
  m_mainShader->UpdateMat4("model", &m_modelMatrix);
  if (m_textureBufferObject)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureBufferObject);
  }
  for (auto& mesh : m_meshData)
  {
    mesh->Bound();
    switch (m_meshType)
    {
      case MeshType::TRIANGLE:
        glDrawElements(GL_TRIANGLES, mesh->IndexCount(), GL_UNSIGNED_INT, 0);
        break;
      case MeshType::POINT:
        glDrawElements(GL_POINTS, mesh->IndexCount(), GL_UNSIGNED_INT, 0);
        break;
      case MeshType::LINE:
        glDrawElements(GL_LINES, mesh->IndexCount(), GL_UNSIGNED_INT, 0);
        break;
      default:
        break;
    }
    mesh->Unbound();
  }
}

}  // namespace libEngine