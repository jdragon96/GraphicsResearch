#pragma once

#include <libEngine/opengl/glMeshBuffer.h>

namespace libEngine
{
//////////////////////////////////////////////////////////////////// MeshBuffer
template <typename VTX_C, typename PXL_C, typename GEOM_C>
glMeshBuffer<VTX_C, PXL_C, GEOM_C>::glMeshBuffer()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
glMeshBuffer<VTX_C, PXL_C, GEOM_C>::~glMeshBuffer()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glMeshBuffer<VTX_C, PXL_C, GEOM_C>::SetTextureBuffer(unsigned int textureBuf)
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glMeshBuffer<VTX_C, PXL_C, GEOM_C>::SetMesh(std::vector<MeshData> mesh)
{
  this->m_meshData.clear();
  for (auto& m : mesh)
  {
    auto meshBuf = glMesh::MakeShared();
    meshBuf->SetData(m);
    this->m_meshData.push_back(meshBuf);
  }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glMeshBuffer<VTX_C, PXL_C, GEOM_C>::Initialize()
{
  for (auto& mesh : this->m_meshData)
  {
    mesh->InitBuffer();
  }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glMeshBuffer<VTX_C, PXL_C, GEOM_C>::Render()
{
  this->m_mainShader->Bound();
  this->m_mainShader->vertexConstBuffer->data.UpdateModel(this->m_modelMatrix);
  this->m_mainShader->vertexConstBuffer->Update();
  if (this->m_texture)
    this->m_texture->Bound();
  for (auto& mesh : this->m_meshData)
  {
    mesh->Bound();
    switch (this->m_meshType)
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
  // if (this->m_renderNornal)
  //{
  //   auto normalEft                                    = glMemoryDB::instance()->normalEffect;
  //   normalEft->geometryConstBuffer->data.model        = this->m_modelMatrix;
  //   normalEft->geometryConstBuffer->data.invTranspose = this->m_mainShader->vertexConstBuffer->data.invTranspose;
  //   normalEft->geometryConstBuffer->data.projection   = this->m_mainShader->vertexConstBuffer->data.projection;
  //   normalEft->geometryConstBuffer->data.view         = this->m_mainShader->vertexConstBuffer->data.view;
  //   normalEft->geometryConstBuffer->data.startColor   = Vec3(1, 0, 0);
  //   normalEft->geometryConstBuffer->data.endColor     = Vec3(0, 0, 1);
  //   normalEft->geometryConstBuffer->data.normalLength = 0.3f;
  //   normalEft->Bound();
  //   normalEft->geometryConstBuffer->Update();
  //   for (auto& mesh : this->m_meshData)
  //   {
  //     mesh->Bound();
  //     glDrawArrays(GL_POINTS, 0, mesh->VertexCount());
  //     mesh->Unbound();
  //   }
  //   normalEft->Unbound();
  // }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glMeshBuffer<VTX_C, PXL_C, GEOM_C>::RenderNormal()
{
}
}  // namespace libEngine