#include "libEngine/API.h"

namespace libEngine
{
API::API()
{
}

ShaderBufferBase<>::SharedPtr API::MakeShader()
{
  ShaderBufferBase<>::SharedPtr shaderPtr;
  switch (MemDB::CurrentEngineType)
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
  switch (MemDB::CurrentEngineType)
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
MeshBufferBase<>::SharedPtr API::MakeMeshBuffer(std::vector<MeshData> data)
{
  MeshBufferBase<>::SharedPtr meshPtr;
  switch (MemDB::CurrentEngineType)
  {
    case EngineType::OPENGL:
      meshPtr = glMeshBuffer<>::MakeShared();
      break;
    case EngineType::DX11:
      meshPtr = dxMeshBuffer<>::MakeShared();
      break;
  }
  meshPtr->SetMesh(data);
  return meshPtr;
}
TextureBufferBase::SharedPtr API::MakeTexture()
{
  TextureBufferBase::SharedPtr texturePtr;
  switch (MemDB::CurrentEngineType)
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
CubeMapBase::SharedPtr API::MakeCubeMap()
{
  CubeMapBase::SharedPtr ptr;
  switch (MemDB::CurrentEngineType)
  {
    case EngineType::OPENGL:
      ptr = glCubemap::MakeShared();
      break;
    case EngineType::DX11:
      ptr = dxCubemap::MakeShared();
      break;
  }

  return ptr;
}
}  // namespace libEngine
