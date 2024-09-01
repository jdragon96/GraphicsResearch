#pragma once

#include <Eigen/dense>

#include <directxtk/DDSTextureLoader.h>  // 큐브맵 읽을 때 필요
#include <directxtk/WICTextureLoader.h>
#include <directxtk/SimpleMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <wrl.h>

#include "libEngine/dx11/dxMeshBuffer.h"
#include "libEngine/dx11/dxShaderBuffer.h"
#include "libEngine/dx11/dxTexture.h"
#include "libEngine/shared/CubeMapBase.h"
#include "libEngine/model/CubemapEffect.h"
#include "libEngine/utils/toybox.h"

namespace libEngine
{
class dxCubemap : public CubeMapBase
{
public:
  SHARED_PTR(dxCubemap);

  dxCubemap();
  ~dxCubemap();

  virtual void Initialize() override;

  virtual void Bound() override;

private:
  dxMeshBuffer<CubemapEffect::VertexConstBuf, CubemapEffect::PixelConstBuf>::SharedPtr m_cubeMeshBuffer;

  dxTexture::SharedPtr                                          m_texture;
  Microsoft::WRL::ComPtr<ID3D11SamplerState>                    m_samplerState;
  std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>>          m_texturesPtr;
  std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_resourceViewsPtr;
  std::vector<ID3D11ShaderResourceView*>                        m_resourceViewsRawPtr;
};
}  // namespace libEngine