#include <libEngine/dx11/dxMesh.h>

#include <d3d11.h>
#include <d3dcompiler.h>

namespace libEngine
{
//////////////////////////////////////////////////////////////////// glMesh
dxMesh::dxMesh() : Mesh()
{
}
dxMesh::~dxMesh()
{
}
void dxMesh::InitBuffer()
{
  auto devPtr = dxRenderer::instance()->GetDevicePtr();

  //   1. Vertex Buffer 생성
  D3D11_BUFFER_DESC bufferDesc;
  ZeroMemory(&bufferDesc, sizeof(bufferDesc));
  bufferDesc.Usage               = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
  bufferDesc.ByteWidth           = UINT(sizeof(Vertex) * VertexCount());
  bufferDesc.BindFlags           = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags      = 0;  // 0 if no CPU access is necessary.
  bufferDesc.StructureByteStride = sizeof(Vertex);

  D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
  vertexBufferData.pSysMem                = m_data.vertices.data();
  vertexBufferData.SysMemPitch            = 0;
  vertexBufferData.SysMemSlicePitch       = 0;

  const HRESULT hr = devPtr->CreateBuffer(&bufferDesc, &vertexBufferData, m_vertexBuffer.GetAddressOf());
  if (FAILED(hr))
  {
    // std::cout << "Vertex CreateBuffer() failed. " << std::hex << hr << std::endl;
  };

  // 2. Index buffer 생성
  D3D11_BUFFER_DESC indexBufferDesc   = {};
  indexBufferDesc.Usage               = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;  // 초기화 후 변경X
  indexBufferDesc.ByteWidth           = UINT(sizeof(uint32_t) * IndexCount());
  indexBufferDesc.BindFlags           = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags      = 0;  // 0 if no CPU access is necessary.
  indexBufferDesc.StructureByteStride = sizeof(uint32_t);

  D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
  indexBufferData.pSysMem                = m_data.indices.data();
  indexBufferData.SysMemPitch            = 0;
  indexBufferData.SysMemSlicePitch       = 0;
  const HRESULT IndexHr = devPtr->CreateBuffer(&indexBufferDesc, &indexBufferData, m_indexBuffer.GetAddressOf());
  if (FAILED(IndexHr))
  {
    // std::cout << "Index CreateBuffer() failed. " << std::hex << hr << std::endl;
  };
}
void dxMesh::Bound()
{
  auto contextPtr = dxRenderer::instance()->GetContextPtr();
  UINT stride     = sizeof(Vertex);
  UINT offset     = 0;
  contextPtr->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  contextPtr->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  contextPtr->DrawIndexed(UINT(IndexCount()), 0, 0);
}
void dxMesh::Unbound()
{
}
}  // namespace libEngine