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
      scOption.BufferDesc.Format                  = DXGI_FORMAT_R16G16B16A16_FLOAT;
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
      D3D11_RASTERIZER_DESC desc;
      ZeroMemory(&desc, sizeof(desc));
      desc.FillMode              = D3D11_FILL_MODE::D3D11_FILL_SOLID;
      desc.CullMode              = D3D11_CULL_MODE::D3D11_CULL_FRONT;
      desc.FrontCounterClockwise = false;
      desc.DepthClipEnable       = true;
      Dx11EngineManager::instance().InitRS(defaultRS, desc);
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
          "../../resource/dx11/BasicVertexShader.hlsl",
          {
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          },
          std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
      defaultPSO->SetPixelShader("../../resource/dx11/BlinnPhongReflect.hlsl");
    }

    // 3. 매쉬 버퍼 생성하기
    {
      auto earthTexturePtr = Dx11TextureBuffer::MakeShared();
      earthTexturePtr->LoadJPG("../../resource/world.jpg");
      auto texturePtr = Dx11TextureBuffer::MakeShared();
      texturePtr->LoadDDS("../../resource/skybox.dds");

      cubeBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
      cubeBuffer->SetPSO(defaultPSO);
      cubeBuffer->SetMesh(MakeSphere(Vec3(1, 0, 0), 1.f, 20, 20));
      cubeBuffer->SetObjectType(ObjectType::CHARACTER);
      cubeBuffer->UseSimulation(false);
      cubeBuffer->SetTexture({ earthTexturePtr, texturePtr });
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

      auto billboardPSO = Dx11GraphicsPSO::MakeShared();
      billboardPSO->SetVertexShader(
          "../../resource/dx11/BillboardVertex.hlsl",
          {
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          },
          std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
      billboardPSO->SetPixelShader("../../resource/dx11/BillboardPixel.hlsl");
      billboardPSO->SetGeometryShader("../../resource/dx11/BillboardGeometryPoint.hlsl");
      billboardPSO->SetObjectType(EObjectBufferType::POINT);

      auto billboardCubePSO = Dx11GraphicsPSO::MakeShared();
      billboardCubePSO->SetVertexShader(
          "../../resource/dx11/BillboardVertex.hlsl",
          {
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
              { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          },
          std::vector<D3D_SHADER_MACRO>{ { "SKINNED", "0" }, { NULL, NULL } });
      billboardCubePSO->SetPixelShader("../../resource/dx11/BillboardPixel.hlsl");
      billboardCubePSO->SetGeometryShader("../../resource/dx11/BillboardGeometryCube.hlsl");
      billboardCubePSO->SetObjectType(EObjectBufferType::POINT);

      BackgroundBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
      BackgroundBuffer->SetPSO(cubemapPSO);
      auto bgMesh = MakeSphere(Vec3(0, 0, 0), 100.f, 20, 20);
      // D3D11_CULL_MODE::D3D11_CULL_NONE으로 텍스쳐를 뒤집을 수 있음
      std::reverse(bgMesh.indices.begin(), bgMesh.indices.end());
      BackgroundBuffer->SetTexture({ texturePtr });
      BackgroundBuffer->SetMesh(bgMesh);
      BackgroundBuffer->SetObjectType(ObjectType::CHARACTER);
      BackgroundBuffer->UseSimulation(false);
      BackgroundBuffer->Initialize();

      {
        BillboardPointBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
        BillboardPointBuffer->SetPSO(billboardPSO);
        MeshData<VertexData> BillboardPoint;
        VertexData           Point;
        Point.SetPosition(0, 0, 3);
        BillboardPoint.vertices.push_back(Point);
        BillboardPoint.indices.push_back({ 0 });
        BillboardPointBuffer->SetTexture({ texturePtr });
        BillboardPointBuffer->SetMesh(BillboardPoint);
        BillboardPointBuffer->UseSimulation(false);
        BillboardPointBuffer->Initialize();
      }

      {
        BillboardCubeBuffer = Dx11MeshBuffer<VertexData>::MakeShared();
        BillboardCubeBuffer->SetPSO(billboardCubePSO);
        MeshData<VertexData> BillboardPoint;
        VertexData           Point;
        Point.SetPosition(0, 0, 5);
        BillboardPoint.vertices.push_back(Point);
        BillboardPoint.indices.push_back({ 0 });
        BillboardCubeBuffer->SetTexture({ texturePtr });
        BillboardCubeBuffer->SetMesh(BillboardPoint);
        BillboardCubeBuffer->UseSimulation(false);
        BillboardCubeBuffer->Initialize();
      }
    }
    {
      commonBuffer = Dx11ConstantBuffer<CCommon>::MakeShared();
      commonBuffer->Initialize(EConstBufferType::VERTEX_GLOBAL);
      blinnPhongBuffer = Dx11ConstantBuffer<CBlinnPhong>::MakeShared();
      blinnPhongBuffer->Initialize(EConstBufferType::PIXEL);
      billboardBuffer = Dx11ConstantBuffer<CBillboardPoint>::MakeShared();
      billboardBuffer->Initialize(EConstBufferType::GEOMETRY);
      BillboardCubeConstBuffer = Dx11ConstantBuffer<CBillboardCube>::MakeShared();
      BillboardCubeConstBuffer->Initialize(EConstBufferType::GEOMETRY);
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
      //
      blinnPhongBuffer->Show();
    };
    Dx11EngineManager::instance().renderFunc = [&]() {
      auto contextPtr = Dx11EngineManager::instance().GetContextPtr();

      const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
      contextPtr->ClearRenderTargetView(backbufferRTV.Get(), clearColor);
      contextPtr->ClearDepthStencilView(defaultDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
      contextPtr->OMSetRenderTargets(1, backbufferRTV.GetAddressOf(), defaultDSV.Get());
      contextPtr->OMSetDepthStencilState(defaultDSS.Get(), 1);
      contextPtr->RSSetState(defaultRS.Get());
      contextPtr->PSSetSamplers(0, 1, defaultSampler.GetAddressOf());

      // 1. 공유 상수버퍼 갱신
      commonBuffer->m_bufferData.view       = mainCamera->GetViewMatPtr()->transpose();
      commonBuffer->m_bufferData.projection = mainCamera->GetProjMatPtr()->transpose();
      commonBuffer->Update();
      commonBuffer->Bind();
      blinnPhongBuffer->m_bufferData.eyeWorld = mainCamera->GetCameraPos();
      blinnPhongBuffer->Update();
      blinnPhongBuffer->Bind();

      // 2.
      BackgroundBuffer->Render();
      cubeBuffer->Render();
      {
        billboardBuffer->m_bufferData.viewGeom = mainCamera->GetViewMatPtr()->transpose();
        billboardBuffer->m_bufferData.projGeom = mainCamera->GetProjMatPtr()->transpose();
        billboardBuffer->Update();
        billboardBuffer->Bind();
        BillboardPointBuffer->Render();
      }
      {
        BillboardCubeConstBuffer->m_bufferData.viewGeom = mainCamera->GetViewMatPtr()->transpose();
        BillboardCubeConstBuffer->m_bufferData.projGeom = mainCamera->GetProjMatPtr()->transpose();
        BillboardCubeConstBuffer->m_bufferData.length   = 1.f;
        BillboardCubeConstBuffer->Update();
        BillboardCubeConstBuffer->Bind();
        BillboardCubeBuffer->Render();
      }
    };

    Dx11EngineManager::instance().Run();
  }

  Dx11GraphicsPSO::SharedPtr defaultPSO;

  Dx11ConstantBuffer<CCommon>::SharedPtr         commonBuffer;
  Dx11ConstantBuffer<CBlinnPhong>::SharedPtr     blinnPhongBuffer;
  Dx11ConstantBuffer<CBillboardPoint>::SharedPtr billboardBuffer;
  Dx11ConstantBuffer<CBillboardCube>::SharedPtr  BillboardCubeConstBuffer;

  Dx11MeshBuffer<VertexData>::SharedPtr cubeBuffer;
  Dx11MeshBuffer<VertexData>::SharedPtr BackgroundBuffer;
  Dx11MeshBuffer<VertexData>::SharedPtr BillboardPointBuffer;
  Dx11MeshBuffer<VertexData>::SharedPtr BillboardCubeBuffer;

  CameraBuffer::SharedPtr                          mainCamera;
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