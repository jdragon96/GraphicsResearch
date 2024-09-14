#pragma once
#include <libEngine/dx11/dxFilter.h>

namespace libEngine
{
template <typename VTX, typename PXL, typename GEOM>
void dxFilter<VTX, PXL, GEOM>::Initialize()
{
  auto devPtr = dxRenderer::instance()->GetDevicePtr();
  {
    // Texture sampler
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD         = 0;
    sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

    // Create the Sample State
    devPtr->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

    // Create a rasterizer state
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));  // Need this
    rastDesc.FillMode              = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rastDesc.CullMode              = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable       = false;

    devPtr->CreateRasterizerState(&rastDesc, m_rasterizerSate.GetAddressOf());

    // Set the viewport
    ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width    = float(this->m_width);
    m_viewport.Height   = float(this->m_height);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width     = this->m_width;
    txtDesc.Height    = this->m_height;
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format                        = DXGI_FORMAT_R32G32B32A32_FLOAT;
    txtDesc.SampleDesc.Count              = 1;
    txtDesc.Usage                         = D3D11_USAGE_DEFAULT;
    txtDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
    txtDesc.MiscFlags      = 0;
    txtDesc.CPUAccessFlags = 0;

    D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
    viewDesc.Format             = txtDesc.Format;
    viewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipSlice = 0;

    devPtr->CreateTexture2D(&txtDesc, NULL, texture.GetAddressOf());
    devPtr->CreateRenderTargetView(texture.Get(), &viewDesc, m_renderTargetView.GetAddressOf());
    devPtr->CreateShaderResourceView(texture.Get(), nullptr, m_shaderResourceView.GetAddressOf());
    SetRenderTargets({ m_renderTargetView });
  }

  // this->m_meshBuffer = API::instance().MakeMeshBuffer<VTX, PXL, GEOM>({ Toybox::MakeSuqare() });
  this->m_meshBuffer = dxMeshBuffer<VTX, PXL, GEOM>::MakeShared();
  this->m_meshBuffer->SetMesh({ Toybox::MakeSuqare() });
  this->m_meshBuffer->SetMeshType(MeshType::TRIANGLE);
  this->m_meshBuffer->SetShader(this->m_shaderBuffer);
  this->m_meshBuffer->Initialize();
}

template <typename VTX, typename PXL, typename GEOM>
void dxFilter<VTX, PXL, GEOM>::Render()
{
  auto ctxPtr = dxRenderer::instance()->GetContextPtr();
  ctxPtr->OMSetRenderTargets(UINT(this->m_renderTargets.size()), this->m_renderTargets.data(), nullptr);
  ctxPtr->RSSetState(this->m_rasterizerSate.Get());
  ctxPtr->RSSetViewports(1, &this->m_viewport);
  ctxPtr->PSSetShaderResources(0, this->m_shaderResources.size(), this->m_shaderResources.data());
  ctxPtr->PSSetSamplers(0, 1, this->m_samplerState.GetAddressOf());
  this->m_meshBuffer->Render();
}

template <typename VTX, typename PXL, typename GEOM>
void dxFilter<VTX, PXL, GEOM>::SetRenderTargets(
    const std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>& targets)
{
  this->m_renderTargets.clear();
  for (const auto& tar : targets)
  {
    this->m_renderTargets.push_back(tar.Get());
  }
}

template <typename VTX, typename PXL, typename GEOM>
void dxFilter<VTX, PXL, GEOM>::SetShaderResources(
    const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& resources)
{
  this->m_shaderResources.clear();
  for (const auto& res : resources)
  {
    this->m_shaderResources.push_back(res.Get());
  }
}

}  // namespace libEngine