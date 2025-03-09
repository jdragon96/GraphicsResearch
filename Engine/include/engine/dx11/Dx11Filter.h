#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "engine/Macro.h"
#include "engine/model/VertexData.h"
#include "engine/model/CFilter.h"
#include "engine/common/ResourceUtils.h"
#include "engine/common/Drawing.h"
#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11MeshBuffer.h"
#include "engine/dx11/Dx11ConstantBuffer.h"

class Dx11Filter
{
public:
  Dx11Filter()
  {
  }
  Dx11Filter(std::string vertex, std::string pixel, int width, int height)
  {
    auto devPtr = Dx11EngineManager::instance().GetDevicePtr();

    // 1. PSO 생성
    {
      m_pso = Dx11GraphicsPSO::MakeShared();
      m_pso->SetVertexShader(
          vertex,
          {
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          },
          std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
      m_pso->SetPixelShader(pixel);
    }

    // 2. Mesh buffer 생성
    {
      m_meshBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
      m_meshBuffer->SetPSO(m_pso);
      m_meshBuffer->SetMesh(MakeSquare(1.0f));
      m_meshBuffer->UseSimulation(false);
      m_meshBuffer->Initialize();
    }

    // 3. 텍스쳐 생성
    {
      D3D11_SAMPLER_DESC sampDesc;
      ZeroMemory(&sampDesc, sizeof(sampDesc));
      sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
      sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
      sampDesc.MinLOD         = 0;
      sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;
      devPtr->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());
    }

    // 4. Rasterization 생성
    {
      D3D11_RASTERIZER_DESC rastDesc;
      ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));  // Need this
      rastDesc.FillMode              = D3D11_FILL_MODE::D3D11_FILL_SOLID;
      rastDesc.CullMode              = D3D11_CULL_MODE::D3D11_CULL_NONE;
      rastDesc.FrontCounterClockwise = false;
      rastDesc.DepthClipEnable       = false;

      devPtr->CreateRasterizerState(&rastDesc, m_rasterizerSate.GetAddressOf());
    }

    // 5. 필터링 결과를 렌더링 할 텍스쳐 생성
    {
      Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

      D3D11_TEXTURE2D_DESC txtDesc;
      ZeroMemory(&txtDesc, sizeof(txtDesc));
      txtDesc.Width     = width;
      txtDesc.Height    = height;
      txtDesc.MipLevels = txtDesc.ArraySize = 1;
      txtDesc.Format                        = DXGI_FORMAT_R32G32B32A32_FLOAT;  //  이미지 처리용도
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
    }
    // 6.  Constant Buffer 생성
    {
      m_constBuffer = Dx11ConstantBuffer<CFilter>::MakeShared();
      m_constBuffer->m_bufferData.dx = 1.f / width;
      m_constBuffer->m_bufferData.dy = 1.f / height;
      m_constBuffer->m_bufferData.threshold = 0.7f;
      m_constBuffer->m_bufferData.strength = 3.f;
      m_constBuffer->Initialize();
    }
    // 7. Viewport 생성
    {
      ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
      m_viewport.TopLeftX = 0;
      m_viewport.TopLeftY = 0;
      m_viewport.Width    = float(width);
      m_viewport.Height   = float(height);
      m_viewport.MinDepth = 0.0f;
      m_viewport.MaxDepth = 1.0f;
    }
    SetRenderTarget({ m_renderTargetView });
  }

  void SetShaderResource(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> resources)
  {
    m_shaderResourceLists.clear();
    for (const auto& res : resources)
    {
      m_shaderResourceLists.push_back(res.Get());
    }
  }

  void SetRenderTarget(std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> targets)
  {
    m_renderTargetLists.clear();
    for (const auto& tar : targets)
    {
      m_renderTargetLists.push_back(tar.Get());
    }
  }

  void Render()
  {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    {
      contextPtr->OMSetRenderTargets(UINT(m_renderTargetLists.size()), m_renderTargetLists.data(), nullptr);
      contextPtr->RSSetState(m_rasterizerSate.Get());
      contextPtr->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
      contextPtr->RSSetViewports(1, &m_viewport);
      contextPtr->PSSetShaderResources(0, UINT(m_shaderResourceLists.size()), m_shaderResourceLists.data());
    }
    {
      m_constBuffer->Update();
      m_constBuffer->Bind(EConstBufferType::FILTER);
      // contextPtr->PSSetConstantBuffers(6, 1, m_constBuffer->GetPtr());
    }
    {
      m_pso->Bind();
      m_meshBuffer->Render();
    }
  }

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_renderTargetView;

protected:
  Dx11GraphicsPSO::SharedPtr             m_pso;
  Dx11MeshBuffer<VertexData>::SharedPtr  m_meshBuffer;
  Dx11ConstantBuffer<CFilter>::SharedPtr m_constBuffer;

  Microsoft::WRL::ComPtr<ID3D11SamplerState>       m_samplerState;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState>    m_rasterizerSate;

  D3D11_VIEWPORT                         m_viewport;
  std::vector<ID3D11RenderTargetView*>   m_renderTargetLists;
  std::vector<ID3D11ShaderResourceView*> m_shaderResourceLists;
};
