#pragma once

#include <Eigen/dense>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <wrl.h>

#include "libEngine/utils/toybox.h"
#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/dx11/dxMeshBuffer.h"
#include "libEngine/shared/FilterBase.h"

namespace libEngine
{
template <typename VTX, typename PXL, typename GEOM>
class dxFilter : public FilterBase<VTX, PXL, GEOM>
{
public:
  SHARED_PTR(dxFilter);

  void SetRenderTargets(const std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>& targets);

  void SetShaderResources(const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& resources);

  virtual void Initialize() override;

  virtual void Render() override;

  std::vector<ID3D11RenderTargetView*>   m_renderTargets;
  std::vector<ID3D11ShaderResourceView*> m_shaderResources;

  D3D11_VIEWPORT                                       m_viewport;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState>        m_rasterizerSate;
  Microsoft::WRL::ComPtr<ID3D11SamplerState>           m_samplerState;
  libEngine::MeshBufferBase<VTX, PXL, GEOM>::SharedPtr m_meshBuffer;

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_renderTargetView;
};
}  // namespace libEngine

#include "libEngine/dx11/dxFilter.tpp"