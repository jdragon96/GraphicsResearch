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

#include "libEngine/shared/TextureBufferBase.h"
#include "libEngine/dx11/dxRenderer.h"

namespace libEngine
{
class dxTexture : public TextureBufferBase
{
public:
  SHARED_PTR(dxTexture);

  dxTexture();
  ~dxTexture();

  virtual void AddImage(std::string path) override;

  virtual void Bound() override;

private:
  Microsoft::WRL::ComPtr<ID3D11SamplerState>                    m_samplerState;
  std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>>          m_texturesPtr;
  std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_resourceViewsPtr;
  std::vector<ID3D11ShaderResourceView*>                        m_resourceViewsRawPtr;
};
}  // namespace libEngine