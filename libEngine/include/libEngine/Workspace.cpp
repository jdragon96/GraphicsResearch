#include "libEngine/Workspace.h"

namespace libEngine
{
Workspace::Workspace()
{
  m_engineType = EngineType::OPENGL;
}
void Workspace::SetEngine(EngineType type)
{
  m_engineType = type;
}
ShaderBufferBase::SharedPtr Workspace::MakeShader()
{
  ShaderBufferBase::SharedPtr shaderPtr;
  switch (m_engineType)
  {
    case EngineType::OPENGL:
      shaderPtr = glShaderBuffer::MakeShared();
      break;
    case EngineType::DX11:
      shaderPtr = dxShaderBuffer::MakeShared();
      break;
  }
  return shaderPtr;
}
CameraBuffer::SharedPtr Workspace::MakeCamera(CameraOption opt)
{
  auto camPtr = CameraBuffer::MakeShared();
  camPtr->SetOption(opt);
  return camPtr;
}
RendererBase::SharedPtr Workspace::MakeRenderer(RendererOption opt)
{
  RendererBase::SharedPtr wsPtr;
  switch (m_engineType)
  {
    case EngineType::OPENGL:
      wsPtr = glRenderer::instance();
      break;
    case EngineType::DX11:
      wsPtr = dxRenderer::instance();
      break;
  }
  wsPtr->SetOption(opt);
  return wsPtr;
}
MeshBufferBase::SharedPtr Workspace::MakeMeshBuffer(std::vector<MeshData> data)
{
  MeshBufferBase::SharedPtr meshPtr;
  switch (m_engineType)
  {
    case EngineType::OPENGL:
      meshPtr = glMeshBuffer::MakeShared();
      break;
    case EngineType::DX11:
      meshPtr = dxMeshBuffer::MakeShared();
      break;
  }
  meshPtr->SetMesh(data);
  return meshPtr;
}
}  // namespace libEngine
