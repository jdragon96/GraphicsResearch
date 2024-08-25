#include "libEngine/shared/MeshBufferBase.h"

namespace libEngine
{
MeshBufferBase::MeshBufferBase() : m_renderNornal(false)
{
  m_modelMatrix.setIdentity();
  m_mainShader = nullptr;
  m_meshType   = MeshType::TRIANGLE;
}
void MeshBufferBase::SetModel(Mat4 model)
{
  m_modelMatrix = model;
}
MeshBufferBase::~MeshBufferBase()
{
}
void MeshBufferBase::Move(Vec3 pos)
{
  m_modelMatrix(3, 0) = pos.x();
  m_modelMatrix(3, 1) = pos.y();
  m_modelMatrix(3, 2) = pos.z();
  m_modelMatrix(3, 3) = 1.f;
}
void MeshBufferBase::SetMeshType(MeshType type)
{
  m_meshType = type;
}

void MeshBufferBase::SetShader(ShaderBufferBase<>::SharedPtr shader)
{
  m_mainShader = shader;
}
void MeshBufferBase::SetTexture(TextureBufferBase::SharedPtr texture)
{
  m_texture = texture;
}
void MeshBufferBase::SetNormalRenderFlag(bool flag)
{
  m_renderNornal = flag;
}
}  // namespace libEngine