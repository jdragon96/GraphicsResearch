#include "libEngine/API.h"

namespace libEngine
{
API::API()
{
  m_engineType = EngineType::OPENGL;
}
void API::SetEngine(EngineType type)
{
  m_engineType = type;
}

ShaderBufferBase<>::SharedPtr API::MakeShader()
{
  ShaderBufferBase<>::SharedPtr shaderPtr;
  switch (m_engineType)
  {
    case EngineType::OPENGL:
      shaderPtr = glShaderBuffer<>::MakeShared();
      break;
    case EngineType::DX11:
      shaderPtr = dxShaderBuffer<>::MakeShared();
      break;
  }
  return shaderPtr;
}
CameraBuffer::SharedPtr API::MakeCamera(CameraOption opt)
{
  auto camPtr = CameraBuffer::MakeShared();
  camPtr->SetOption(opt);
  return camPtr;
}
RendererBase::SharedPtr API::MakeRenderer(RendererOption opt)
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
MeshBufferBase::SharedPtr API::MakeMeshBuffer(std::vector<MeshData> data)
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
TextureBufferBase::SharedPtr API::MakeTexture()
{
  TextureBufferBase::SharedPtr texturePtr;
  switch (m_engineType)
  {
    case EngineType::OPENGL:
      texturePtr = glTexture::MakeShared();
      break;
    case EngineType::DX11:
      texturePtr = dxTexture::MakeShared();
      break;
  }

  return texturePtr;
}
}  // namespace libEngine
