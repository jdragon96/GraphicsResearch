#pragma once

#include <libEngine/dx11/dxMeshBuffer.h>

namespace libEngine
{
//////////////////////////////////////////////////////////////////// MeshBuffer
template <typename VTX_C, typename PXL_C, typename GEOM_C>
dxMeshBuffer<VTX_C, PXL_C, GEOM_C>::dxMeshBuffer() : MeshBufferBase<VTX_C, PXL_C, GEOM_C>()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
dxMeshBuffer<VTX_C, PXL_C, GEOM_C>::~dxMeshBuffer()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxMeshBuffer<VTX_C, PXL_C, GEOM_C>::SetTextureBuffer(unsigned int textureBuf)
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxMeshBuffer<VTX_C, PXL_C, GEOM_C>::SetMesh(std::vector<MeshData> mesh)
{
  this->m_meshData.clear();
  for (auto& m : mesh)
  {
    auto meshBuf = dxMesh::MakeShared();
    meshBuf->SetData(m);
    this->m_meshData.push_back(meshBuf);
  }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxMeshBuffer<VTX_C, PXL_C, GEOM_C>::Initialize()
{
  for (auto& mesh : this->m_meshData)
  {
    mesh->InitBuffer();
  }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxMeshBuffer<VTX_C, PXL_C, GEOM_C>::Render()
{
  auto contextPtr = dxRenderer::instance()->GetContextPtr();
  this->m_mainShader->vertexConstBuffer->data.UpdateModel(this->m_modelMatrix);
  // this->m_mainShader->vertexConstBuffer->data.invTranspose = this->m_modelMatrix.inverse();
  this->m_mainShader->Bound();
  this->m_mainShader->vertexConstBuffer->Update();
  if (this->m_texture)
    this->m_texture->Bound();
  for (auto& mesh : this->m_meshData)
  {
    switch (this->m_meshType)
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
  this->m_mainShader->Unbound();
  // Normal 랜더링하는 경우
  // if (this->m_renderNornal)
  //{
  //  auto normalEffectPtr = dxMemoryDB::instance()->normalEffect;
  //  normalEffectPtr->geometryConstBuffer->data.UpdateModel(this->m_modelMatrix);
  //  normalEffectPtr->geometryConstBuffer->data.view         = this->m_mainShader->vertexConstBuffer->data.view;
  //  normalEffectPtr->geometryConstBuffer->data.projection   = this->m_mainShader->vertexConstBuffer->data.projection;
  //  normalEffectPtr->geometryConstBuffer->data.normalLength = 0.3f;
  //  normalEffectPtr->geometryConstBuffer->data.startColor   = Vec3(1, 0, 0);
  //  normalEffectPtr->geometryConstBuffer->data.endColor     = Vec3(0, 0, 1);
  //  normalEffectPtr->Bound();
  //  normalEffectPtr->geometryConstBuffer->Update();
  //  for (auto& mesh : this->m_meshData)
  //  {
  //    contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
  //    contextPtr->Draw(UINT(mesh->VertexCount()), 0);
  //  }
  //  normalEffectPtr->Unbound();
  //}
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxMeshBuffer<VTX_C, PXL_C, GEOM_C>::RenderNormal()
{
}
}  // namespace libEngine