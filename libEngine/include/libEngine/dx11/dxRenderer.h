#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <windows.h>
#include <wrl.h>

#include "libEngine/utils/macro.h"
#include "libEngine/shared/RendererBase.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>                        // DXGIFactory
#include <dxgi1_4.h>                     // DXGIFactory4
#include <directxtk/DDSTextureLoader.h>  // 큐브맵 읽을 때 필요

namespace libEngine
{
class dxRenderer : public RendererBase
{
public:
  SINGLETON_PTR(dxRenderer);
  // SHARED_PTR(dxRenderer);

  dxRenderer();
  ~dxRenderer();

  virtual void Initialize() override;

  virtual void Run() override;

  void InitRenderingTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>&   renderTarget,
                           Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderResource,
                           Microsoft::WRL::ComPtr<ID3D11Texture2D>&          buffer);

  void InitViewPort(D3D11_VIEWPORT& viewPort);

  void InitRasterizerState(Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rasterizerState,
                           D3D11_FILL_MODE                                mode = D3D11_FILL_MODE::D3D11_FILL_SOLID,
                           D3D11_CULL_MODE                                cull = D3D11_CULL_MODE::D3D11_CULL_NONE);

  void InitDepthBuffer(Microsoft::WRL::ComPtr<ID3D11Texture2D>&        depthBuffer,
                       Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& depthView);

  void InitDepthStencilState(Microsoft::WRL::ComPtr<ID3D11DepthStencilState>& depthState);

  void InitSampler(Microsoft::WRL::ComPtr<ID3D11SamplerState>& sampler);

  Microsoft::WRL::ComPtr<ID3D11Device> GetDevicePtr()
  {
    return m_device;
  }

  Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContextPtr()
  {
    return m_context;
  }

  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GetDepthState()
  {
    return m_mainDepthState;
  }

public:
  void InitWindow();

  void InitDeviceAndSwapChain();

  HWND                                        m_mainWindow;
  Microsoft::WRL::ComPtr<ID3D11Device>        m_device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
  Microsoft::WRL::ComPtr<IDXGISwapChain>      m_swapChain;
  // Render target setting
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_mainRenderTarget;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_mainShaderResource;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_renderBuffer;
  // main viewport
  D3D11_VIEWPORT m_mainScreenViewport;
  // main vertex fill mode
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_mainRasterizationMode;
  // depth buffer setting
  Microsoft::WRL::ComPtr<ID3D11Texture2D>        m_mainDepthBuffer;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_mainDepthView;
  // depth state setting
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_mainDepthState;
  // texture sampler
  Microsoft::WRL::ComPtr<ID3D11SamplerState> m_mainSamplerState;

  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tempTexture;
};
}  // namespace libEngine