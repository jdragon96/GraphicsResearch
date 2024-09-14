#pragma once
#include "libEngine/API.h"

namespace libEngine
{
API::API()
{
}

template <typename VTX, typename PXL, typename GEOM>
ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr API::MakeShader()
{
  typename ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr shaderPtr;
  switch (MemDB::CurrentEngineType)
  {
    case EngineType::OPENGL:
      shaderPtr = glShaderBuffer<VTX, PXL, GEOM>::MakeShared();
      break;
    case EngineType::DX11:
      shaderPtr = dxShaderBuffer<VTX, PXL, GEOM>::MakeShared();
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

template <typename VTX, typename PXL, typename GEOM>
MeshBufferBase<VTX, PXL, GEOM>::SharedPtr API::MakeMeshBuffer(std::vector<MeshData> data)
{
  typename MeshBufferBase<VTX, PXL, GEOM>::SharedPtr meshPtr;
  switch (MemDB::CurrentEngineType)
  {
    case EngineType::OPENGL:
      meshPtr = glMeshBuffer<VTX, PXL, GEOM>::MakeShared();
      break;
    case EngineType::DX11:
      meshPtr = dxMeshBuffer<VTX, PXL, GEOM>::MakeShared();
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

PostProcessingBase::SharedPtr API::MakeBloomEffect()
{
  PostProcessingBase::SharedPtr ptr;
  switch (MemDB::CurrentEngineType)
  {
    case EngineType::OPENGL:
      ptr = glBloomEffect::MakeShared();
      break;
    case EngineType::DX11:
      ptr = dxBloomEffect::MakeShared();
      break;
  }
  return ptr;
}
}  // namespace libEngine
