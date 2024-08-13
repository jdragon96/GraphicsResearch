#include "libEngine/shared/MeshBufferBase.h"

namespace libEngine
{
MeshBufferBase::MeshBufferBase()
{
  m_modelMatrix.setIdentity();
  m_mainShader = nullptr;
  m_meshType   = MeshType::TRIANGLE;
}

MeshBufferBase::~MeshBufferBase()
{
}

void MeshBufferBase::SetMeshType(MeshType type)
{
  m_meshType = type;
}

void MeshBufferBase::SetShader(ShaderBufferBase::SharedPtr shader)
{
  m_mainShader = shader;
}
}  // namespace libEngine