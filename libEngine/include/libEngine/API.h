#pragma once

#include <memory>
#include <vector>

#include "libEngine/MemDB.h"

#include "libEngine/utils/macro.h"

#include "libEngine/shared/RendererBase.h"
#include "libEngine/shared/MeshBufferBase.h"
#include "libEngine/shared/CameraBuffer.h"
#include "libEngine/shared/PostProcessingBase.h"

#include "libEngine/opengl/glRenderer.h"
#include "libEngine/opengl/glMeshBuffer.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/opengl/glTexture.h"
#include "libEngine/opengl/glCubemap.h"
#include "libEngine/opengl/postprocessing/glBloomEffect.h"

#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/dx11/dxMeshBuffer.h"
#include "libEngine/dx11/dxShaderBuffer.h"
#include "libEngine/dx11/dxTexture.h"
#include "libEngine/dx11/dxCubemap.h"
#include "libEngine/dx11/postprocessing/dxBloomEffect.h"

namespace libEngine
{
class API
{
public:
  SINGLETON(API);

  API();

  template <typename VTX = BlinnPhong::VertexShaderModel, typename PXL = BlinnPhong::PixelShaderModel,
            typename GEOM = BlinnPhong::GeometryShaderModel>
  ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr MakeShader();

  CameraBuffer::SharedPtr MakeCamera(CameraOption opt);

  RendererBase::SharedPtr MakeRenderer(RendererOption opt);

  template <typename VTX = BlinnPhong::VertexShaderModel, typename PXL = BlinnPhong::PixelShaderModel,
            typename GEOM = BlinnPhong::GeometryShaderModel>
  MeshBufferBase<VTX, PXL, GEOM>::SharedPtr MakeMeshBuffer(std::vector<MeshData> data);

  TextureBufferBase::SharedPtr MakeTexture();

  CubeMapBase::SharedPtr MakeCubeMap();

  PostProcessingBase::SharedPtr MakeBloomEffect();

  RendererBase::SharedPtr m_renderer;
};
}  // namespace libEngine

#include "libEngine/API.tpp"