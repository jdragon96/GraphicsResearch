#include "engine/dx11/Dx11EnginePSO.h"
#include <filesystem>

Dx11EnginePSO::Dx11EnginePSO() : PipelineObjectBase()
{
  D3D11_DEPTH_STENCIL_DESC dsDesc;
  ZeroMemory(&dsDesc, sizeof(dsDesc));
  dsDesc.DepthEnable      = true;
  dsDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
  dsDesc.DepthFunc        = D3D11_COMPARISON_LESS;
  dsDesc.StencilEnable    = false;  // Stencil 불필요
  dsDesc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
  dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
  // 앞면에 대해서 어떻게 작동할지 설정
  dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
  // 뒷면에 대해 어떻게 작동할지 설정 (뒷면도 그릴 경우)
  dsDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
  dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
  dsDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

  auto devPtr = Dx11EngineManager::instance().GetDevicePtr();
  Dx11EngineManager::Check(devPtr->CreateDepthStencilState(&dsDesc, m_drawDSS.GetAddressOf()));

  // Stencil에 1로 표기해주는 DSS
  dsDesc.DepthEnable      = true;  // 이미 그려진 물체 유지
  dsDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO;
  dsDesc.DepthFunc        = D3D11_COMPARISON_LESS;
  dsDesc.StencilEnable    = true;  // Stencil 필수
  dsDesc.StencilReadMask  = 0xFF;  // 모든 비트 다 사용
  dsDesc.StencilWriteMask = 0xFF;  // 모든 비트 다 사용
  // 앞면에 대해서 어떻게 작동할지 설정
  dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
  dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

  Dx11EngineManager::Check(devPtr->CreateDepthStencilState(&dsDesc, m_maskDSS.GetAddressOf()));

  // Stencil에 1로 표기된 경우에"만" 그리는 DSS
  // DepthBuffer는 초기화된 상태로 가정
  // D3D11_COMPARISON_EQUAL 이미 1로 표기된 경우에만 그리기
  // OMSetDepthStencilState(..., 1); <- 여기의 1
  dsDesc.DepthEnable                  = true;  // 거울 속을 다시 그릴때 필요
  dsDesc.StencilEnable                = true;  // Stencil 사용
  dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
  dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;  // <- 주의
  dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_EQUAL;

  Dx11EngineManager::Check(devPtr->CreateDepthStencilState(&dsDesc, m_drawMaskedDSS.GetAddressOf()));

  /* "이미 그려져있는 화면"과 어떻게 섞을지를 결정
   * Dest: 이미 그려져 있는 값들을 의미
   * Src: 픽셀 쉐이더가 계산한 값들을 의미 (여기서는 마지막 거울)
   */
  D3D11_BLEND_DESC mirrorBlendDesc;
  ZeroMemory(&mirrorBlendDesc, sizeof(mirrorBlendDesc));
  mirrorBlendDesc.AlphaToCoverageEnable  = true;  // MSAA
  mirrorBlendDesc.IndependentBlendEnable = false;
  // 개별 RenderTarget에 대해서 설정 (최대 8개)
  mirrorBlendDesc.RenderTarget[0].BlendEnable = true;
  mirrorBlendDesc.RenderTarget[0].SrcBlend    = D3D11_BLEND_INV_BLEND_FACTOR;
  mirrorBlendDesc.RenderTarget[0].DestBlend   = D3D11_BLEND_BLEND_FACTOR;
  mirrorBlendDesc.RenderTarget[0].BlendOp     = D3D11_BLEND_OP_ADD;

  mirrorBlendDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
  mirrorBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  mirrorBlendDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;

  // 필요하면 RGBA 각각에 대해서도 조절 가능
  mirrorBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  Dx11EngineManager::Check(devPtr->CreateBlendState(&mirrorBlendDesc, m_mirrorBS.GetAddressOf()));

  D3D11_RASTERIZER_DESC rastDesc;
  ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
  rastDesc.FillMode              = D3D11_FILL_MODE::D3D11_FILL_SOLID;
  rastDesc.CullMode              = D3D11_CULL_MODE::D3D11_CULL_FRONT;
  rastDesc.FrontCounterClockwise = true;
  rastDesc.DepthClipEnable       = true;
  rastDesc.MultisampleEnable     = true;
  Dx11EngineManager::Check(devPtr->CreateRasterizerState(&rastDesc, m_solidRS.GetAddressOf()));
  rastDesc.FillMode              = D3D11_FILL_MODE::D3D11_FILL_SOLID;
  rastDesc.CullMode              = D3D11_CULL_MODE::D3D11_CULL_BACK;
  rastDesc.FrontCounterClockwise = false;
  Dx11EngineManager::Check(devPtr->CreateRasterizerState(&rastDesc, m_solidCCWRS.GetAddressOf()));

  D3D11_TEXTURE2D_DESC dsOption;
  ZeroMemory(&dsOption, sizeof(dsOption));
  dsOption.Width              = Dx11EngineManager::instance().m_screenOption.width;
  dsOption.Height             = Dx11EngineManager::instance().m_screenOption.height;
  dsOption.MipLevels          = 1;
  dsOption.ArraySize          = 1;
  dsOption.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
  dsOption.Usage              = D3D11_USAGE_DEFAULT;
  dsOption.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
  dsOption.CPUAccessFlags     = 0;
  dsOption.MiscFlags          = 0;
  dsOption.SampleDesc.Count   = 1;
  dsOption.SampleDesc.Quality = 0;
  Dx11EngineManager::instance().InitTexture(dsOption, m_depthTexture);
  Dx11EngineManager::instance().InitDSV(m_depthTexture, m_depthStencilView);

  D3D11_SAMPLER_DESC samplerOption;
  ZeroMemory(&samplerOption, sizeof(samplerOption));
  samplerOption.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerOption.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerOption.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerOption.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerOption.ComparisonFunc = D3D11_COMPARISON_NEVER;
  samplerOption.MinLOD         = 0;
  samplerOption.MaxLOD         = D3D11_FLOAT32_MAX;
  Dx11EngineManager::instance().InitSampler(m_linearSampler, samplerOption);
}
void Dx11EnginePSO::DrawCounterClock()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->RSSetState(m_solidCCWRS.Get());
}
void Dx11EnginePSO::DrawClock()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->RSSetState(m_solidRS.Get());
}
void Dx11EnginePSO::ClearDepth()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void Dx11EnginePSO::ClearStencil()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Dx11EnginePSO::ClearDepthStencil()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void Dx11EnginePSO::EnableStencil()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->OMSetDepthStencilState(m_maskDSS.Get(), 1);
}
void Dx11EnginePSO::EnableDepth()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->OMSetDepthStencilState(m_drawDSS.Get(), 1);
}
void Dx11EnginePSO::WriteDpethAndStencil()
{
}
void Dx11EnginePSO::EnableStencilMask()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->OMSetDepthStencilState(m_drawMaskedDSS.Get(), 1);
}
void Dx11EnginePSO::EnableSampler()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->PSSetSamplers(0, 1, m_linearSampler.GetAddressOf());
  contextPtr->VSSetSamplers(0, 1, m_linearSampler.GetAddressOf());
  contextPtr->CSSetSamplers(0, 1, m_linearSampler.GetAddressOf());
}
Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Dx11EnginePSO::GetDefaultDSV()
{
  return m_depthStencilView;
}