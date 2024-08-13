#include <libEngine/dx11/dxMeshBuffer.h>

namespace libEngine
{
//////////////////////////////////////////////////////////////////// MeshBuffer
dxMeshBuffer::dxMeshBuffer()
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
  auto contextPtr = dxRenderer::instance()->GetContextPtr();
  m_mainShader->Bound();
  m_mainShader->UpdateMat4("model", &m_modelMatrix);
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
}

}  // namespace libEngine