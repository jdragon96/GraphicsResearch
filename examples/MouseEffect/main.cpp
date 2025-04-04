#include <vector>
#include <chrono>
#include <numeric>
#include <random>

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

struct Particle
{
  Vec3 Position;
  Vec3 Color;
};

struct MouseStatus
{
  Vec2 pos;
  int  isMousePressed = 0;
  int  dummy1;
};

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

  auto GPUData = Dx11ComputeBuffer::MakeShared();
  GPUData->Initialize(512, 512);

  auto MousePSO = Dx11GraphicsPSO::MakeShared();
  MousePSO->SetVertexShaderCode(
      R"(
cbuffer MouseCursorConstBuffer : register(b5)
{
  float2 cursorPos;
  int    isMousePressed;
  int    dummy1;
};

struct VSInput
{
    float3 vertexPos : POSITION0;
    float4 vertexColor: COLOR0;
    float2 vertexTexCoord : TEXTURECOORD0;
    float3 vertexNormal : NORMAL0;
};

struct GSInput
{
    float4 pos: SV_POSITION;
};

GSInput main(VSInput input)
{
    GSInput output;
    //output.pos.x = 0;
    //output.pos.y = 0;
    output.pos.x = cursorPos.x;
    output.pos.y = cursorPos.y;
    output.pos.z = 1.f;
    output.pos.w = 1.f;
    return output;
}
  )",
      {
          { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      },
      std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
  MousePSO->SetGeometryShaderCode(
      R"(
struct GeometryShaderInput
{
    float4 pos : SV_POSITION;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 color : COLOR;
    uint primID : SV_PrimitiveID;
};

[maxvertexcount(4)]
void main(point GeometryShaderInput input[1], uint primID : SV_PrimitiveID,
                              inout TriangleStream<PixelShaderInput> outputStream)
{
    const float width = 0.1;
    
    float hw = 0.3 * width;
    float3 up = float3(0, 1, 0);
    float3 right = float3(1, 0, 0);
    
    PixelShaderInput output;
    output.pos.w = 1;
    output.color = float3(1,0,0);
    
    output.pos.xyz = input[0].pos.xyz - hw * right - hw*up;
    output.texCoord = float2(0.0, 1.0);
    output.primID = primID;
    
    outputStream.Append(output);

    output.pos.xyz = input[0].pos.xyz - hw * right + hw * up;
    output.texCoord = float2(0.0, 0.0);
    output.primID = primID;
    
    outputStream.Append(output);
    
    output.pos.xyz = input[0].pos.xyz + hw * right - hw * up;
    output.texCoord = float2(1.0, 1.0);
    output.primID = primID;
    
    outputStream.Append(output);
    
    output.pos.xyz = input[0].pos.xyz + hw * right + hw * up;
    output.texCoord = float2(1.0, 0.0);
    output.primID = primID;

    outputStream.Append(output);
    outputStream.RestartStrip();
}

)");
  MousePSO->SetPixelShaderCode(
      R"(
Texture2D g_baseTexture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer MouseCursorConstBuffer : register(b5)
{
  float2 cursorPos;
  int    isMousePressed;
  int    dummy1;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 color : COLOR;
    uint primID : SV_PrimitiveID;
};

float smootherstep(float x, float edge0 = 0.0f, float edge1 = 1.0f)
{
    x = clamp((x - edge0) / (edge1 - edge0), 0, 1);
    return x * x * x * (3 * x * (2 * x - 5) + 10.0f);
}

float4 main(PixelShaderInput input) : SV_TARGET
{
  if(!isMousePressed)
  {
    clip(-1);
  }    
  float dist = length(float2(0.5, 0.5) - input.texCoord) * 2;
  float scale = smootherstep(1 - dist); 
  return float4(float3(1,0,0) * scale, 1);
}
    )");
  MousePSO->SetComputeShaderCode(R"(
RWTexture2D<float4> densityField : register(u0);

[numthreads(16, 16, 1)]
void main(int3 gID : SV_GroupID, int3 gtID : SV_GroupThreadID, uint3 dtID : SV_DispatchThreadID)
{
    // 시간에 따라 컬러값을 감소
    const float dissipation = 0.02f;
    float3 color = densityField[dtID.xy].rgb - dissipation;
    color = max(0, color);
    densityField[dtID.xy] = float4(color, 1.0);
}
      )");
  D3D11_VIEWPORT m_screenViewport;

  auto                 MouseEventBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
  MeshData<VertexData> MouseMesh;
  auto                 pointVertexData = VertexData();
  MouseMesh.vertices.push_back(pointVertexData);
  MouseMesh.indices.push_back(0);
  MouseEventBuffer->SetMesh(MouseMesh);
  MouseEventBuffer->UseSimulation(false);
  MouseEventBuffer->SetPSO(MousePSO);
  MouseEventBuffer->Initialize();

  auto MouseEventConstBuffer = Dx11ConstantBuffer<MouseStatus>::MakeShared();
  MouseEventConstBuffer->Initialize(EConstBufferType::VERTEX_MODEL);

  auto DensityTexture = Dx11TextureBuffer::MakeShared();
  DensityTexture->CreateUATexture(Dx11EngineManager::instance().m_screenOption.height,
                                  Dx11EngineManager::instance().m_screenOption.width);

  Microsoft::WRL::ComPtr<ID3D11BlendState> accumulateBS;
  D3D11_BLEND_DESC                         blendDesc;
  ZeroMemory(&blendDesc, sizeof(blendDesc));
  blendDesc.AlphaToCoverageEnable                 = false;  // MSAA
  blendDesc.IndependentBlendEnable                = false;
  blendDesc.RenderTarget[0].BlendEnable           = true;
  blendDesc.RenderTarget[0].SrcBlend              = D3D11_BLEND_BLEND_FACTOR;
  blendDesc.RenderTarget[0].DestBlend             = D3D11_BLEND_BLEND_FACTOR;  // INV 아님
  blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
  blendDesc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ONE;
  blendDesc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  auto devicePtr                                  = Dx11EngineManager::instance().GetDevicePtr();
  Dx11EngineManager::Check(devicePtr->CreateBlendState(&blendDesc, accumulateBS.GetAddressOf()));

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
    {
      // ID3D11RenderTargetView* nullRTVs[1] = { nullptr };
      // contextPtr->OMSetRenderTargets(1, nullRTVs, nullptr);
      MousePSO->Bind();
      // 1. Texture 활성화 시키기
      DensityTexture->UseUAV(0);
      // 2. Compute shader 호출하기
      MousePSO->RenderCompute(UINT(ceil(m_screenViewport.Width / 16.f)), UINT(ceil(m_screenViewport.Height / 16.f)), 1);
      Dx11EngineManager::BarriorCompute();
      // 3. 마우스 데이터 갱신
      MouseEventConstBuffer->m_bufferData.pos[0] = (Dx11EngineManager::instance().m_currentPosX - 256.f) / 256.f;
      MouseEventConstBuffer->m_bufferData.pos[1] = (256.f - Dx11EngineManager::instance().m_currentPosY) / 256.f;
      MouseEventConstBuffer->m_bufferData.isMousePressed = Dx11EngineManager::instance().IsPressed();
      MouseEventConstBuffer->Update();
      MouseEventConstBuffer->BindAll(5);
      // 4. 텍스쳐 사용
      Dx11EnginePSO::instance().EnableSampler();
      // 5. 랜더링
      DensityTexture->UseRenderTarget(1);
      contextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
      const float blendColor[4] = { 1.f, 1.f, 1.f, 1.f };
      contextPtr->OMSetBlendState(accumulateBS.Get(), blendColor, 0xffffffff);
      contextPtr->Draw(UINT(1), 0);
      contextPtr->OMSetRenderTargets(1, backbufferRTV.GetAddressOf(), nullptr);
      DensityTexture->CopyTo(backTexture);
    }
  };
  Dx11EngineManager::instance().Run();
}

int main()
{
  ParticleExample();
  return 0;
}