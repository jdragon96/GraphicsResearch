#include "Particle.h"

void ParticleExample()
{
  CameraBuffer::SharedPtr                          mainCamera;
  Dx11GraphicsPSO::SharedPtr                       ShaderPSO;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   backbufferRTV;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backbufferSRV;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          backTexture;

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

  auto GPUData = Dx11ComputeBuffer::MakeShared();
  GPUData->Initialize(512, 512);

  ShaderPSO = Dx11GraphicsPSO::MakeShared();
  ShaderPSO->SetVertexShader("../../resource/dx11/StructedBufferVertexShader.hlsl",
                             {
                                 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                             },
                             {});
  ShaderPSO->SetPixelShader("../../resource/dx11/StructedBufferPixelShader.hlsl");
  ShaderPSO->SetComputeShaderCode(R"(
struct Particle
{
    float3 pos;
    float3 color;
};

static float dt = 1 / 60.0;
RWStructuredBuffer<Particle> outputParticles : register(u0);

[numthreads(1024, 1, 1)]
void main(int3 gID : SV_GroupID, int3 gtID : SV_GroupThreadID, uint3 dtID : SV_DispatchThreadID)
{
    Particle p = outputParticles[dtID.x];

    float cosTheta = cos(dt);
    float sinTheta = sin(dt);

    float3 newPos;
    newPos.x = p.pos.x * cosTheta - p.pos.y * sinTheta;
    newPos.y = p.pos.x * sinTheta + p.pos.y * cosTheta;
    newPos.z = p.pos.z;

    outputParticles[dtID.x].pos = newPos;
}
    )");
  // circleComputePSO->SetComputeShader("../resource/dx11/CreateCircleComputeShader.hlsl");

  auto                                  ParticleBuffer = Dx11StructedBuffer<Particle>::MakeShared();
  std::vector<Particle>                 particles;
  std::mt19937                          gen(0);
  std::uniform_real_distribution<float> dp(-1.0f, 1.0f);
  std::uniform_real_distribution<float> dc(0.1f, 1.0f);
  particles.resize(25600);
  for (auto& p : particles)
  {
    p.Position = Vec3(dp(gen), dp(gen), 1.0f);
    p.Color    = Vec3(dc(gen), dc(gen), dc(gen));
  }
  ParticleBuffer->Initialize(particles);

  D3D11_VIEWPORT m_screenViewport;
  Dx11EnginePSO::instance();
  Dx11EngineManager::instance().InitImGui();
  Dx11EngineManager::instance().imguiFunc  = [&]() {};
  Dx11EngineManager::instance().renderFunc = [&]() {
    auto contextPtr = Dx11EngineManager::instance().GetContextPtr();

    ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
    m_screenViewport.TopLeftX = 0;
    m_screenViewport.TopLeftY = 0;
    m_screenViewport.Width    = float(512);
    m_screenViewport.Height   = float(512);
    m_screenViewport.MinDepth = 0.0f;
    m_screenViewport.MaxDepth = 1.0f;
    contextPtr->RSSetViewports(1, &m_screenViewport);
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    contextPtr->ClearRenderTargetView(backbufferRTV.Get(), clearColor);
    contextPtr->OMSetRenderTargets(1, backbufferRTV.GetAddressOf(), nullptr);

    {
      ShaderPSO->SetObjectType(EObjectBufferType::POINTLIST);
      ShaderPSO->Bind();
      ParticleBuffer->UseComputeUAV(0);
      ParticleBuffer->RenderCompute(256, 1, 1);
      ParticleBuffer->UseVertexSRV(0);
      ParticleBuffer->RenderVertex();
    }
    {
      // GPUData->CopyTo(backTexture);
    }
  };
  Dx11EngineManager::instance().Run();
}
