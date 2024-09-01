#pragma once

#include "libEngine/shared/MeshBufferBase.h"

namespace libEngine
{
template <typename VTX_C, typename PXL_C, typename GEOM_C>
MeshBufferBase<VTX_C, PXL_C, GEOM_C>::MeshBufferBase() : m_renderNornal(false)
{
  m_modelMatrix.setIdentity();
  m_mainShader = nullptr;
  m_meshType   = MeshType::TRIANGLE;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void MeshBufferBase<VTX_C, PXL_C, GEOM_C>::SetModel(Mat4 model)
{
  m_modelMatrix = model;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
MeshBufferBase<VTX_C, PXL_C, GEOM_C>::~MeshBufferBase()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void MeshBufferBase<VTX_C, PXL_C, GEOM_C>::Move(Vec3 pos)
{
  m_modelMatrix(3, 0) = pos.x();
  m_modelMatrix(3, 1) = pos.y();
  m_modelMatrix(3, 2) = pos.z();
  m_modelMatrix(3, 3) = 1.f;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void MeshBufferBase<VTX_C, PXL_C, GEOM_C>::SetMeshType(MeshType type)
{
  m_meshType = type;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void MeshBufferBase<VTX_C, PXL_C, GEOM_C>::SetShader(ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::SharedPtr shader)
{
  m_mainShader = shader;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void MeshBufferBase<VTX_C, PXL_C, GEOM_C>::SetTexture(TextureBufferBase::SharedPtr texture)
{
  m_texture = texture;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void MeshBufferBase<VTX_C, PXL_C, GEOM_C>::SetNormalRenderFlag(bool flag)
{
  m_renderNornal = flag;
}
}  // namespace libEngine