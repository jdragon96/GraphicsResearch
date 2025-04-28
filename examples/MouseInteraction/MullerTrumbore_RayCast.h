#pragma once

#include <vector>
#include <chrono>
#include <numeric>
#include <random>
#include <algorithm>

#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11MeshBuffer.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "engine/dx11/Dx11Filter.h"
#include "engine/common/CameraBuffer.h"
#include "engine/common/Drawing.h"
#include "engine/model/CBlinnPhong.h"
#include "engine/model/CCommon.h"
#include "engine/model/CBillboardPoint.h"
#include "engine/model/CBillboardCube.h"
#include "engine/model/CGlobalPixel.h"
#include "engine/model/CTextures.h"
#include "engine/dx11/Dx11EnginePSO.h"
#include "engine/dx11/Dx11ComputeBuffer.h"
#include "engine/dx11/Dx11StructedBuffer.h"
#include "engine/common/Ray.h"
#include "engine/common/AABB.h"

void MullerTrumbore_RayCast()
{
  CameraBuffer::SharedPtr                          mainCamera;
  Dx11GraphicsPSO::SharedPtr                       ShaderPSO;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   backbufferRTV;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backbufferSRV;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          backTexture;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   defaultDSV;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          depthTexture;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  defaultDSS;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState>    defaultRS;

  // 1. 카메라 세팅
  {
    CameraOption camOpt;
    camOpt.sensitivity = 0.001f;
    camOpt.SetPerspective(45, 1280.f / 800, 0.01f, 1000.f);
    camOpt.SetView(Vec3(0.f, 3.f, 0.f), Vec3(0, 0, 0), Vec3(0, 0, 1));

    mainCamera = CameraBuffer::MakeShared();
    mainCamera->SetOption(camOpt);
    mainCamera->Initialize();

    RendererOption option;
    option.height = 512;
    option.width  = 512;
    option.title  = "Engine Test";
    Dx11EngineManager::instance().SetCamera(mainCamera);
    Dx11EngineManager::instance().SetScreenOption(option);
    Dx11EngineManager::instance().InitWindow();
  }
  {
    // 2. 스왑체인 생성
    DXGI_SWAP_CHAIN_DESC scOption;
    ZeroMemory(&scOption, sizeof(scOption));
    scOption.BufferDesc.Format                  = DXGI_FORMAT_R16G16B16A16_FLOAT;
    scOption.BufferCount                        = 2;
    scOption.BufferDesc.RefreshRate.Numerator   = 60;
    scOption.BufferDesc.RefreshRate.Denominator = 1;
    scOption.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    scOption.Windowed                           = TRUE;
    scOption.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scOption.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
    scOption.OutputWindow                       = Dx11EngineManager::instance().m_mainWindow;
    scOption.BufferDesc.Width                   = Dx11EngineManager::instance().m_screenOption.width;
    scOption.BufferDesc.Height                  = Dx11EngineManager::instance().m_screenOption.height;
    Dx11EngineManager::instance().InitDeviceAndSwapChain(scOption);
  }
  {
    // 3. backbuffer의 rtv와 srv를 생성
    Dx11EngineManager::instance().TextureFromBack(backTexture);
    Dx11EngineManager::instance().InitRTV(backTexture, backbufferRTV);
    Dx11EngineManager::instance().InitSRV(backTexture, backbufferSRV);
  }
  {
    // Depth stencil view 생성하기
    // - 스크린의 depth를 측정할 수 있는 버퍼
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
    Dx11EngineManager::instance().InitTexture(dsOption, depthTexture);
    Dx11EngineManager::instance().InitDSV(depthTexture, defaultDSV);
  }
  {
    D3D11_DEPTH_STENCIL_DESC dssOption;
    ZeroMemory(&dssOption, sizeof(dssOption));
    dssOption.DepthEnable                  = true;
    dssOption.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
    dssOption.DepthFunc                    = D3D11_COMPARISON_LESS;
    dssOption.StencilEnable                = false;  // Stencil 불필요
    dssOption.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
    dssOption.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dssOption.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    dssOption.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dssOption.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    dssOption.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    dssOption.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    dssOption.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    dssOption.BackFace.StencilPassOp       = D3D11_STENCIL_OP_REPLACE;
    dssOption.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
    Dx11EngineManager::instance().InitDSS(dssOption, defaultDSS);
  }
  {
    D3D11_RASTERIZER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.FillMode              = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    desc.CullMode              = D3D11_CULL_MODE::D3D11_CULL_FRONT;
    desc.FrontCounterClockwise = false;
    desc.DepthClipEnable       = true;
    Dx11EngineManager::instance().InitRS(defaultRS, desc);
  }

  auto GPUData = Dx11ComputeBuffer::MakeShared();
  GPUData->Initialize(512, 512);

  D3D11_VIEWPORT m_screenViewport;

  // 2. Mouse로 picking 할 3D Object
  auto ObjectPSD      = Dx11GraphicsPSO::MakeShared();
  auto Objectbuffer   = Dx11MeshBuffer<VertexData>::MakeShared();
  auto tracePosBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
  auto commonBuffer   = Dx11ConstantBuffer<CCommon>::MakeShared();
  Vec3 minBBOX;
  Vec3 maxBBOX;
  {
    commonBuffer->Initialize(EConstBufferType::VERTEX_GLOBAL);

    ObjectPSD->SetVertexShader(
        "../../resource/dx11/BasicVertexShader.hlsl",
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        },
        std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
    ObjectPSD->SetPixelShader("../../resource/dx11/BasicPixelShader.hlsl");
    ObjectPSD->SetObjectType(EObjectBufferType::TRIANGLE);

    Objectbuffer->SetMesh(MakeSphere(Vec3(1, 0, 1), 1, 30, 60));
    Objectbuffer->UseSimulation(false);
    Objectbuffer->SetPSO(ObjectPSD);
    Objectbuffer->Initialize();

    tracePosBuffer->SetMesh(MakeSphere(Vec3(1, 0, 0), 0.04, 30, 50));
    tracePosBuffer->UseSimulation(false);
    tracePosBuffer->SetPSO(ObjectPSD);
    tracePosBuffer->Initialize();
  }

  // 3. Ray
  auto ray = Ray::MakeShared();

  Dx11EnginePSO::instance();
  Dx11EngineManager::instance().InitImGui();
  Dx11EngineManager::instance().imguiFunc  = [&]() {};
  Dx11EngineManager::instance().renderFunc = [&]() {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
    {
      // 1. ViewPort 설정하기
      ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
      m_screenViewport.TopLeftX = 0;
      m_screenViewport.TopLeftY = 0;
      m_screenViewport.Width    = float(512);
      m_screenViewport.Height   = float(512);
      m_screenViewport.MinDepth = 0.0f;
      m_screenViewport.MaxDepth = 1.0f;
      contextPtr->RSSetViewports(1, &m_screenViewport);
    }
    {
      // 2. Renter Target 설정하기
      // 2.3. Render target buffer 초기화
      const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
      contextPtr->ClearRenderTargetView(backbufferRTV.Get(), clearColor);
      // 2.1. Depth/Stencil 버퍼 초기화
      contextPtr->ClearDepthStencilView(defaultDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
      // 2.2. Dpeth, stencil 조건값 설정
      contextPtr->OMSetDepthStencilState(defaultDSS.Get(), 1);
      // 2.4. Depth, Stencil buffer와 Renter target buffer 설정
      contextPtr->OMSetRenderTargets(1, backbufferRTV.GetAddressOf(), defaultDSV.Get());
      // 2.4. Rasterization 설정
      contextPtr->RSSetState(defaultRS.Get());
      // 2.5. 디폴트 샘플러 등록하기
      Dx11EnginePSO::instance().EnableLinearSampler(0);
    }
    {
      commonBuffer->m_bufferData.view       = mainCamera->GetViewMatPtr()->transpose();
      commonBuffer->m_bufferData.projection = mainCamera->GetProjMatPtr()->transpose();
      commonBuffer->Update();
      commonBuffer->Bind();
    }
    {
      // 3. ray casting
      ray->UpdateRay(mainCamera,
                     Vec2(Dx11EngineManager::instance().m_currentPosX, Dx11EngineManager::instance().m_currentPosY),
                     512, 512);
      Objectbuffer->Render();
      ray->Trace(Objectbuffer);
      if (ray->MoveTracePos(tracePosBuffer))
      {
        tracePosBuffer->Render();
      }
    }
  };
  Dx11EngineManager::instance().Run();
}