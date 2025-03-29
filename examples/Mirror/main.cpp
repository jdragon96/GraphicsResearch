#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11MeshBuffer.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "engine/dx11/Dx11TextureBuffer.h"
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
#include "CCubemap.h"
#include "CPBRPixel.h"

class DrawBlinnPhongRender
{
public:
  DrawBlinnPhongRender()
  {
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
      option.height = 800;
      option.width  = 1280;
      option.title  = "Engine Test";

      Dx11EngineManager::instance().SetCamera(mainCamera);
      Dx11EngineManager::instance().SetScreenOption(option);
      Dx11EngineManager::instance().InitWindow();
    }

    // 2. 스왑체인 생성
    {
      DXGI_SWAP_CHAIN_DESC scOption;
      ZeroMemory(&scOption, sizeof(scOption));
      // scOption.BufferDesc.Format                  = DXGI_FORMAT_R16G16B16A16_FLOAT;
      //  MASS 사용 시 float으로 변경
      scOption.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
      scOption.BufferCount                        = 2;
      scOption.BufferDesc.RefreshRate.Numerator   = 60;
      scOption.BufferDesc.RefreshRate.Denominator = 1;
      scOption.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_UNORDERED_ACCESS;
      scOption.Windowed     = TRUE;
      scOption.Flags        = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
      scOption.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
      scOption.OutputWindow = Dx11EngineManager::instance().m_mainWindow;
      scOption.BufferDesc.Width  = Dx11EngineManager::instance().m_screenOption.width;
      scOption.BufferDesc.Height = Dx11EngineManager::instance().m_screenOption.height;
      Dx11EngineManager::instance().InitDeviceAndSwapChain(scOption);
    }

    {
      Microsoft::WRL::ComPtr<ID3D11Texture2D> backTexture;
      Dx11EngineManager::instance().TextureFromBack(backTexture);
      Dx11EngineManager::instance().InitRTV(backTexture, backbufferRTV);
      Dx11EngineManager::instance().InitSRV(backTexture, backbufferSRV);
    }
    {
      D3D11_SAMPLER_DESC samplerOption;
      ZeroMemory(&samplerOption, sizeof(samplerOption));
      samplerOption.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      samplerOption.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
      samplerOption.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
      samplerOption.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
      samplerOption.ComparisonFunc = D3D11_COMPARISON_NEVER;
      samplerOption.MinLOD         = 0;
      samplerOption.MaxLOD         = D3D11_FLOAT32_MAX;
      Dx11EngineManager::instance().InitSampler(defaultSampler, samplerOption);
    }
    {
      defaultPSO = Dx11GraphicsPSO::MakeShared();
      defaultPSO->SetVertexShader(
          "../../resource/dx11/HDRVertexShader.hlsl",
          {
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          },
          std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
      defaultPSO->SetPixelShader("../../resource/dx11/HDRPixelShader.hlsl");
    }

    // 3. 매쉬 버퍼 생성하기
    {
      // auto StagingTexture = Dx11TextureBuffer::MakeShared();
      // StagingTexture->CreateStagingTexture("../../resource/RoofingTiles011C_4K-JPG/RoofingTiles011C_4K-JPG_Color.jpg",
      //                                      0, 1);
      // auto earthTexturePtr = Dx11TextureBuffer::MakeShared();
      // earthTexturePtr->CreateMipmapTexture(StagingTexture);
      auto albedoTexturePtr = Dx11TextureBuffer::MakeShared();
      albedoTexturePtr->CreateShaderResourceTexture(
          "../../resource/RoofingTiles011C_4K-JPG/RoofingTiles011C_4K-JPG_Color.jpg");
      auto normalTexturePtr = Dx11TextureBuffer::MakeShared();
      normalTexturePtr->CreateShaderResourceTexture(
          "../../resource/RoofingTiles011C_4K-JPG/RoofingTiles011C_4K-JPG_NormalDX.jpg");
      auto aoTexturePtr = Dx11TextureBuffer::MakeShared();
      aoTexturePtr->CreateShaderResourceTexture(
          "../../resource/RoofingTiles011C_4K-JPG/RoofingTiles011C_4K-JPG_AmbientOcclusion.jpg");
      auto roughnessTexturePtr = Dx11TextureBuffer::MakeShared();
      roughnessTexturePtr->CreateShaderResourceTexture(
          "../../resource/RoofingTiles011C_4K-JPG/RoofingTiles011C_4K-JPG_Roughness.jpg");
      auto heightTexturePtr = Dx11TextureBuffer::MakeShared();
      heightTexturePtr->CreateShaderResourceTexture(
          "../../resource/RoofingTiles011C_4K-JPG/RoofingTiles011C_4K-JPG_Displacement.jpg");

      auto cubemapColor = Dx11TextureBuffer::MakeShared();
      cubemapColor->LoadDDS("../../resource/HDRI/EnvironmentEnvHDR.dds");
      auto cubemapSpecularColor = Dx11TextureBuffer::MakeShared();
      cubemapSpecularColor->LoadDDS("../../resource/HDRI/EnvironmentSpecularHDR.dds");
      auto cubemapDiffuseColor = Dx11TextureBuffer::MakeShared();
      cubemapDiffuseColor->LoadDDS("../../resource/HDRI/EnvironmentDiffuseHDR.dds");
      auto cubemapBRDF = Dx11TextureBuffer::MakeShared();
      cubemapBRDF->LoadDDS("../../resource/HDRI/EnvironmentBrdf.dds", false);

      cubeBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
      cubeBuffer->SetPSO(defaultPSO);
      cubeBuffer->SetMesh(MakeSphere(Vec3(1, 0, 0), 1.f, 40, 40));
      cubeBuffer->SetObjectType(ObjectType::CHARACTER);
      cubeBuffer->UseSimulation(false);
      cubeBuffer->SetVertexTexture({ heightTexturePtr });
      cubeBuffer->SetPixelTexture({ cubemapColor, cubemapDiffuseColor, cubemapBRDF, albedoTexturePtr, normalTexturePtr,
                                    aoTexturePtr, roughnessTexturePtr, roughnessTexturePtr });
      cubeBuffer->Initialize();

      auto cubemapPSO = Dx11GraphicsPSO::MakeShared();
      cubemapPSO->SetVertexShader(
          "../../resource/dx11/BasicVertexShader.hlsl",
          {
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          },
          std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
      cubemapPSO->SetPixelShader("../../resource/dx11/CubemapPixelShader.hlsl");
      cubemapPSO->SetObjectType(EObjectBufferType::TRIANGLE);

      auto mirrorPSO = Dx11GraphicsPSO::MakeShared();
      mirrorPSO->SetVertexShader(
          "../../resource/dx11/BasicVertexShader.hlsl",
          {
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          },
          std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
      mirrorPSO->SetPixelShader("../../resource/dx11/MirrorPixelShader.hlsl");
      mirrorPSO->SetObjectType(EObjectBufferType::TRIANGLE);

      BackgroundBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
      BackgroundBuffer->SetPSO(cubemapPSO);
      auto bgMesh = MakeSphere(Vec3(0, 0, 0), 100.f, 20, 20);
      // D3D11_CULL_MODE::D3D11_CULL_NONE으로 텍스쳐를 뒤집을 수 있음
      std::reverse(bgMesh.indices.begin(), bgMesh.indices.end());
      BackgroundBuffer->SetPixelTexture({ cubemapColor, cubemapSpecularColor, cubemapDiffuseColor });
      BackgroundBuffer->SetMesh(bgMesh);
      BackgroundBuffer->SetObjectType(ObjectType::CHARACTER);
      BackgroundBuffer->UseSimulation(false);
      BackgroundBuffer->Initialize();

      mirrorBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
      mirrorBuffer->SetPSO(mirrorPSO);
      mirrorBuffer->SetMesh(MakeSquare(5));
      mirrorBuffer->SetObjectType(ObjectType::CHARACTER);
      mirrorBuffer->UseSimulation(false);
      mirrorBuffer->SetPixelTexture({});
      mirrorBuffer->Initialize();
      mirrorBuffer->SetPosition(Vec3(0, 0, 5));
    }
    {
      commonBuffer = Dx11ConstantBuffer<CCommon>::MakeShared();
      commonBuffer->Initialize(EConstBufferType::VERTEX_GLOBAL);
      blinnPhongBuffer = Dx11ConstantBuffer<CBlinnPhong>::MakeShared();
      blinnPhongBuffer->Initialize(EConstBufferType::PIXEL);
      GlobalPixelConstBuffer = Dx11ConstantBuffer<CGlobalPixel>::MakeShared();
      GlobalPixelConstBuffer->Initialize(EConstBufferType::PIXEL_GLOBAL);
      texturePixelConstBuffer = Dx11ConstantBuffer<CTextures>::MakeShared();
      texturePixelConstBuffer->Initialize(EConstBufferType::PIXEL);
      cubemapTextureConstBuffer = Dx11ConstantBuffer<CCubemap>::MakeShared();
      cubemapTextureConstBuffer->Initialize(EConstBufferType::PIXEL);
      pbrPixelConstBuffer = Dx11ConstantBuffer<CPBRPixel>::MakeShared();
      pbrPixelConstBuffer->Initialize(EConstBufferType::PIXEL);
    }
    Dx11EngineManager::instance().InitImGui();
  }

  void Run()
  {
    blinnPhongBuffer->m_bufferData.light.position     = Vec3(-1.6, 1.6, 1.3);
    blinnPhongBuffer->m_bufferData.light.direction    = Vec3(-0.5, -0.1, -0.4);
    blinnPhongBuffer->m_bufferData.mat.ambient        = Vec3(-0.16, 0.19, 0.64);
    blinnPhongBuffer->m_bufferData.mat.ambientFactor  = 0.2;
    blinnPhongBuffer->m_bufferData.mat.diffuse        = Vec3(0, 0.1, 0.1);
    blinnPhongBuffer->m_bufferData.mat.shininess      = 0.0;
    blinnPhongBuffer->m_bufferData.mat.specular       = Vec3(1, 1, 1);
    blinnPhongBuffer->m_bufferData.mat.specularFactor = 1.3;

    Dx11EngineManager::instance().imguiFunc = [&]() {
      // texturePixelConstBuffer->Show();
      GlobalPixelConstBuffer->Show();
      blinnPhongBuffer->Show();
      commonBuffer->Show();
      cubemapTextureConstBuffer->Show();
      pbrPixelConstBuffer->Show();
    };
    Dx11EngineManager::instance().renderFunc = [&]() {
      auto contextPtr = Dx11EngineManager::instance().GetContextPtr();

      const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
      contextPtr->ClearRenderTargetView(backbufferRTV.Get(), clearColor);
      contextPtr->OMSetRenderTargets(1, backbufferRTV.GetAddressOf(), Dx11EnginePSO::instance().GetDefaultDSV().Get());
      Dx11EnginePSO::instance().ClearDepthStencil();
      Dx11EnginePSO::instance().EnableDepth();
      Dx11EnginePSO::instance().DrawClock();
      contextPtr->VSSetSamplers(0, 1, defaultSampler.GetAddressOf());
      contextPtr->PSSetSamplers(0, 1, defaultSampler.GetAddressOf());
      contextPtr->OMSetBlendState(NULL, NULL, 0xffffffff);

      // 1. 공유 상수버퍼 갱신
      GlobalPixelConstBuffer->m_bufferData.eyeWorld = mainCamera->GetCameraPos();
      GlobalPixelConstBuffer->m_bufferData.time += ImGui::GetIO().DeltaTime;
      GlobalPixelConstBuffer->Update();
      GlobalPixelConstBuffer->Bind();
      commonBuffer->m_bufferData.view       = mainCamera->GetViewMatPtr()->transpose();
      commonBuffer->m_bufferData.projection = mainCamera->GetProjMatPtr()->transpose();
      commonBuffer->Update();
      commonBuffer->Bind();
      {
        cubemapTextureConstBuffer->Update();
        cubemapTextureConstBuffer->Bind();
        BackgroundBuffer->Render();
      }
      {
        pbrPixelConstBuffer->Update();
        pbrPixelConstBuffer->Bind(5);
        blinnPhongBuffer->Update();
        blinnPhongBuffer->Bind(6);
        cubeBuffer->Render();
      }
      {
        Dx11EnginePSO::instance().ClearStencil();
        Dx11EnginePSO::instance().EnableStencil();
        mirrorBuffer->Render();
        Dx11EnginePSO::instance().EnableStencilMask();
        Dx11EnginePSO::instance().DrawCounterClock();
        cubeBuffer->Reflect(mirrorBuffer->GetPosition(), Vec3(0, 0, -1));
      }
    };
    Dx11EngineManager::instance().Run();
  }

  Dx11GraphicsPSO::SharedPtr defaultPSO;

  Dx11ConstantBuffer<CCommon>::SharedPtr      commonBuffer;
  Dx11ConstantBuffer<CBlinnPhong>::SharedPtr  blinnPhongBuffer;
  Dx11ConstantBuffer<CGlobalPixel>::SharedPtr GlobalPixelConstBuffer;
  Dx11ConstantBuffer<CTextures>::SharedPtr    texturePixelConstBuffer;
  Dx11ConstantBuffer<CCubemap>::SharedPtr     cubemapTextureConstBuffer;
  Dx11ConstantBuffer<CPBRPixel>::SharedPtr    pbrPixelConstBuffer;

  Dx11MeshBuffer<VertexData>::SharedPtr cubeBuffer;
  Dx11MeshBuffer<VertexData>::SharedPtr BackgroundBuffer;
  Dx11MeshBuffer<VertexData>::SharedPtr mirrorBuffer;
  CameraBuffer::SharedPtr               mainCamera;

  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   backbufferRTV;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backbufferSRV;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   PostProcessingBuffer;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   defaultDSV;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>          depthTexture;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState>    defaultRS;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  defaultDSS;
  Microsoft::WRL::ComPtr<ID3D11SamplerState>       defaultSampler;
};

void main()
{
  DrawBlinnPhongRender program;
  program.Run();
  return;
}