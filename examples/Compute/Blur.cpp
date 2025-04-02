#include "Blur.h"

void CircleExample()
{
  CameraBuffer::SharedPtr                          mainCamera;
  Dx11GraphicsPSO::SharedPtr                       circleComputePSO;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   backbufferRTV;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backbufferSRV;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          backTexture;
  Dx11ConstantBuffer<CircleConstBuffer>::SharedPtr constBuffer;

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
  // 2. 스왑체인 생성
  {
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
    Dx11EngineManager::instance().TextureFromBack(backTexture);
    Dx11EngineManager::instance().InitRTV(backTexture, backbufferRTV);
    Dx11EngineManager::instance().InitSRV(backTexture, backbufferSRV);
  }

  auto TextureFromUAV = Dx11TextureBuffer::MakeShared();

  constBuffer = Dx11ConstantBuffer<CircleConstBuffer>::MakeShared();
  constBuffer->Initialize(EConstBufferType::COMPUTE);

  auto blurConstBuffer = Dx11ConstantBuffer<BlurConstBuffer>::MakeShared();
  blurConstBuffer->Initialize(EConstBufferType::COMPUTE);

  auto GPUData = Dx11ComputeBuffer::MakeShared();
  GPUData->Initialize(512, 512);

  circleComputePSO = Dx11GraphicsPSO::MakeShared();
  circleComputePSO->SetComputeShader("../resource/dx11/CreateCircleComputeShader.hlsl");
  auto blurPSO = Dx11GraphicsPSO::MakeShared();
  blurPSO->SetComputeShader("../resource/dx11/BlurComputeShader.hlsl");

  Dx11EnginePSO::instance();
  Dx11EngineManager::instance().InitImGui();
  Dx11EngineManager::instance().imguiFunc  = [&]() { constBuffer->Show(); };
  Dx11EngineManager::instance().renderFunc = [&]() {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();

    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    contextPtr->ClearRenderTargetView(backbufferRTV.Get(), clearColor);
    contextPtr->OMSetRenderTargets(1, backbufferRTV.GetAddressOf(), nullptr);
    Dx11EnginePSO::instance().EnableSampler();
    {
      circleComputePSO->Bind();
      constBuffer->Update();
      constBuffer->Bind();
      GPUData->Execute(16, 16, 1);
    }
    for (int i = 0; i < 10; ++i)
    {
      {
        TextureFromUAV->CopyFrom(GPUData);
        blurPSO->Bind();
        blurConstBuffer->m_bufferData.useHorizontal = 1;
        blurConstBuffer->Update();
        blurConstBuffer->Bind();
        GPUData->SetTexture({ TextureFromUAV->textureResourceView.Get() });
        GPUData->Execute(16, 16, 1);
      }
      {
        TextureFromUAV->CopyFrom(GPUData);
        blurConstBuffer->m_bufferData.useHorizontal = 0;
        blurConstBuffer->Update();
        blurConstBuffer->Bind();
        GPUData->SetTexture({ TextureFromUAV->textureResourceView.Get() });
        GPUData->Execute(16, 16, 1);
      }
    }
    {
      GPUData->CopyTo(backTexture);
    }
  };
  Dx11EngineManager::instance().Run();
}
