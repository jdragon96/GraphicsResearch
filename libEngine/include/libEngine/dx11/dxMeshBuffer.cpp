#include <libEngine/dx11/dxMeshBuffer.h>

namespace libEngine
{
//////////////////////////////////////////////////////////////////// MeshBuffer
dxMeshBuffer::dxMeshBuffer() : MeshBufferBase()
{
}
dxMeshBuffer::~dxMeshBuffer()
{
}
void dxMeshBuffer::SetTextureBuffer(unsigned int textureBuf)
{
}
void dxMeshBuffer::SetMesh(std::vector<MeshData> mesh)
{
  m_meshData.clear();
  for (auto& m : mesh)
  {
    auto meshBuf = dxMesh::MakeShared();
    meshBuf->SetData(m);
    m_meshData.push_back(meshBuf);
  }
}
void dxMeshBuffer::Initialize()
{
  for (auto& mesh : m_meshData)
  {
    mesh->InitBuffer();
  }
}
void dxMeshBuffer::Render()
{
  auto contextPtr                                    = dxRenderer::instance()->GetContextPtr();
  m_mainShader->vertexConstBuffer->data.model        = m_modelMatrix.transpose();
  m_mainShader->vertexConstBuffer->data.invTranspose = m_modelMatrix.inverse();
  m_mainShader->Bound();
  m_mainShader->vertexConstBuffer->Update();
  if (m_texture)
    m_texture->Bound();
  for (auto& mesh : m_meshData)
  {
    switch (m_meshType)
    {
      case MeshType::TRIANGLE:
        contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;
      case MeshType::LINE:
        contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        break;
      case MeshType::POINT:
        contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        break;
    }
    mesh->Bound();
    mesh->Unbound();
  }
  m_mainShader->Unbound();
  // Normal 랜더링하는 경우
  if (m_renderNornal)
  {
    auto normalEffectPtr                                    = dxMemoryDB::instance()->normalEffect;
    normalEffectPtr->geometryConstBuffer->data.model        = m_mainShader->vertexConstBuffer->data.model;
    normalEffectPtr->geometryConstBuffer->data.invTranspose = m_mainShader->vertexConstBuffer->data.invTranspose;
    normalEffectPtr->geometryConstBuffer->data.view         = m_mainShader->vertexConstBuffer->data.view;
    normalEffectPtr->geometryConstBuffer->data.projection   = m_mainShader->vertexConstBuffer->data.projection;
    normalEffectPtr->geometryConstBuffer->data.normalLength = 0.3f;
    normalEffectPtr->geometryConstBuffer->data.startColor   = Vec3(1, 0, 0);
    normalEffectPtr->geometryConstBuffer->data.endColor     = Vec3(0, 0, 1);
    normalEffectPtr->Bound();
    normalEffectPtr->geometryConstBuffer->Update();
    for (auto& mesh : m_meshData)
    {
      contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
      contextPtr->Draw(UINT(mesh->VertexCount()), 0);
    }
    normalEffectPtr->Unbound();
  }
}
void dxMeshBuffer::RenderNormal()
{
}
}  // namespace libEngine