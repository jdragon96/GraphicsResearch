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
  m_mainShader->vertexConstBuffer->data.model        = m_modelMatrix;
  m_mainShader->vertexConstBuffer->data.invTranspose = m_modelMatrix.inverse().transpose();
  m_mainShader->vertexConstBuffer->Update();
  if (m_texture)
    m_texture->Bound();
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
  if (m_renderNornal)
  {
    auto normalEft                                    = glMemoryDB::instance()->normalEffect;
    normalEft->geometryConstBuffer->data.model        = m_modelMatrix;
    normalEft->geometryConstBuffer->data.invTranspose = m_mainShader->vertexConstBuffer->data.invTranspose;
    normalEft->geometryConstBuffer->data.projection   = m_mainShader->vertexConstBuffer->data.projection;
    normalEft->geometryConstBuffer->data.view         = m_mainShader->vertexConstBuffer->data.view;
    normalEft->geometryConstBuffer->data.startColor   = Vec3(1, 0, 0);
    normalEft->geometryConstBuffer->data.endColor     = Vec3(0, 0, 1);
    normalEft->geometryConstBuffer->data.normalLength = 0.3f;
    normalEft->Bound();
    normalEft->geometryConstBuffer->Update();
    for (auto& mesh : m_meshData)
    {
      mesh->Bound();
      glDrawArrays(GL_POINTS, 0, mesh->VertexCount());
      mesh->Unbound();
    }
    normalEft->Unbound();
  }
}
void glMeshBuffer::RenderNormal()
{
}
}  // namespace libEngine