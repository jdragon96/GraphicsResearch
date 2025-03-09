#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <windows.h>
#include <wrl.h>
#include <WinUser.h>
#include <iostream>

#include "engine/Macro.h"
#include "engine/common/CameraBuffer.h"
#include "engine/interface/EngineBase.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>                        // DXGIFactory
#include <dxgi1_4.h>                     // DXGIFactory4
#include <directxtk/DDSTextureLoader.h>  // ť��� ���� �� �ʿ�

#include "physx/PxPhysicsAPI.h"
#include "physx/PxActor.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

static physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0,
                                                      physx::PxFilterData             filterData0,
                                                      physx::PxFilterObjectAttributes attributes1,
                                                      physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags,
                                                      const void* constantBlock, physx::PxU32 constantBlockSize)
{
  PX_UNUSED(attributes0);
  PX_UNUSED(attributes1);
  PX_UNUSED(filterData0);
  PX_UNUSED(filterData1);
  PX_UNUSED(constantBlockSize);
  PX_UNUSED(constantBlock);

  // all initial and persisting reports for everything, with per-point data
  pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT |
              physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS |
              physx::PxPairFlag::eNOTIFY_CONTACT_POINTS | physx::PxPairFlag::eMODIFY_CONTACTS;
  return physx::PxFilterFlag::eDEFAULT;
}

class MySimulationEventCallback : public physx::PxSimulationEventCallback
{
public:
  void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
  {
    PX_UNUSED(constraints);
    PX_UNUSED(count);
  }
  void onWake(physx::PxActor** actors, physx::PxU32 count)
  {
    PX_UNUSED(actors);
    PX_UNUSED(count);
  }
  void onSleep(physx::PxActor** actors, physx::PxU32 count)
  {
    PX_UNUSED(actors);
    PX_UNUSED(count);
  }
  void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
  {
    PX_UNUSED(pairs);
    PX_UNUSED(count);
  }
  void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32)
  {
  }

  virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs,
                         physx::PxU32 nbPairs) override
  {
    // 충돌한 객체 A와 B 가져오기
    physx::PxActor* actorA = pairHeader.actors[0];
    physx::PxActor* actorB = pairHeader.actors[1];

    // 특정 조건에 따라 삭제 대상 추가
    if (actorA && actorB)
    {
      // std::cout << actorA->getName() << ", " << actorB->getName() << std::endl;
      auto name1 = actorA->getName();
      auto name2 = actorB->getName();
      {
        if (name1 == "GROUND" || name2 == "GROUND")
          return;
        if (name1 == "WEAPON" || name2 == "WEAPON")
        {
          if (name1 == "ENEMY")
            actorsToRemove.push_back(actorA);
          else if (name2 == "ENEMY")
            actorsToRemove.push_back(actorB);
        }
      }
      frame++;
    }
  }

  bool isTargetActor(physx::PxActor* actor)
  {
    return false;
  }

  std::vector<physx::PxActor*> actorsToRemove;

  int frame = 0;
};

class Dx11EngineManager : public EngineBase
{
public:
  SINGLETON(Dx11EngineManager);

  Dx11EngineManager();
  ~Dx11EngineManager();

  void SetCamera(CameraBuffer::SharedPtr cam)
  {
    m_camPtr = cam;
  }

  // void SetScreenOption(RendererOption option);

  virtual void Initialize();

  virtual void Run();

  void InitVP(D3D11_VIEWPORT& viewPort);
  void InitRS(Microsoft::WRL::ComPtr<ID3D11RasterizerState>& rs, D3D11_RASTERIZER_DESC desc);
  void InitSampler(Microsoft::WRL::ComPtr<ID3D11SamplerState>& sampler, D3D11_SAMPLER_DESC desc);
  void InitDSS(D3D11_DEPTH_STENCIL_DESC desc, Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ptr);
  void InitDB(Microsoft::WRL::ComPtr<ID3D11Texture2D>&        depthBuffer,
              Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& depthView, D3D11_TEXTURE2D_DESC ds);
  void InitWindow();
  void InitDeviceAndSwapChain(DXGI_SWAP_CHAIN_DESC desc);

  void ClearDSV(int flag, float depth, float stencil);

  void TextureFromBack(Microsoft::WRL::ComPtr<ID3D11Texture2D>& texturePtr)
  {
    Check(m_swapChain->GetBuffer(0, IID_PPV_ARGS(texturePtr.GetAddressOf())));
  }
  void InitRTV(Microsoft::WRL::ComPtr<ID3D11Texture2D>&        texturePtr,
               Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& rtvPtr)
  {
    Check(m_device->CreateRenderTargetView(texturePtr.Get(), NULL, rtvPtr.GetAddressOf()));
  }
  void InitSRV(Microsoft::WRL::ComPtr<ID3D11Texture2D>&          texturePtr,
               Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& rtvPtr)
  {
    Check(m_device->CreateShaderResourceView(texturePtr.Get(), NULL, rtvPtr.GetAddressOf()));
  }
  void InitTexture(D3D11_TEXTURE2D_DESC desc, Microsoft::WRL::ComPtr<ID3D11Texture2D>& texturePtr)
  {
    Check(m_device->CreateTexture2D(&desc, nullptr, texturePtr.GetAddressOf()));
  }
  void InitDSV(Microsoft::WRL::ComPtr<ID3D11Texture2D>&        depthBuffer,
               Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& dvPtr)
  {
    Check(m_device->CreateDepthStencilView(depthBuffer.Get(), 0, &dvPtr));
  }

  Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContextPtr()
  {
    return m_context;
  }
  Microsoft::WRL::ComPtr<ID3D11Device> GetDevicePtr()
  {
    return m_device;
  }

  void CreateDDSTexture()
  {
  }

  static void CreateStructuredBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, const UINT numElements,
                                     const UINT sizeElement, const void* initData,
                                     Microsoft::WRL::ComPtr<ID3D11Buffer>&              buffer,
                                     Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>&  srv,
                                     Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& uav)
  {
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage     = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = numElements * sizeElement;
    bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS |  // Compute Shader
                           D3D11_BIND_SHADER_RESOURCE;    // Vertex Shader
    bufferDesc.StructureByteStride = sizeElement;
    bufferDesc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    // 참고: Structured는 D3D11_BIND_VERTEX_BUFFER로 사용 불가
    if (initData)
    {
      D3D11_SUBRESOURCE_DATA bufferData;
      ZeroMemory(&bufferData, sizeof(bufferData));
      bufferData.pSysMem = initData;
      device->CreateBuffer(&bufferDesc, &bufferData, buffer.GetAddressOf());
    }
    else
    {
      device->CreateBuffer(&bufferDesc, NULL, buffer.GetAddressOf());
    }

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    ZeroMemory(&uavDesc, sizeof(uavDesc));
    uavDesc.Format             = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension      = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.NumElements = numElements;
    device->CreateUnorderedAccessView(buffer.Get(), &uavDesc, uav.GetAddressOf());

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format               = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension        = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.BufferEx.NumElements = numElements;
    device->CreateShaderResourceView(buffer.Get(), &srvDesc, srv.GetAddressOf());
  }

  static void CreateStagingBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device, const UINT numElements,
                                  const UINT sizeElement, const void* initData,
                                  Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer)
  {
    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth           = numElements * sizeElement;
    desc.Usage               = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
    desc.StructureByteStride = sizeElement;

    if (initData)
    {
      D3D11_SUBRESOURCE_DATA bufferData;
      ZeroMemory(&bufferData, sizeof(bufferData));
      bufferData.pSysMem = initData;
      device->CreateBuffer(&desc, &bufferData, buffer.GetAddressOf());
    }
    else
    {
      device->CreateBuffer(&desc, NULL, buffer.GetAddressOf());
    }
  }
  static void CopyToStagingBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context,
                                  Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, UINT size, void* src)
  {
    D3D11_MAPPED_SUBRESOURCE ms;
    context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE, NULL, &ms);
    memcpy(ms.pData, src, size);
    context->Unmap(buffer.Get(), NULL);
  }
  static void CopyFromStagingBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context,
                                    Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, UINT size, void* dest)
  {
    D3D11_MAPPED_SUBRESOURCE ms;
    context->Map(buffer.Get(), NULL, D3D11_MAP_READ, NULL, &ms);
    memcpy(dest, ms.pData, size);
    context->Unmap(buffer.Get(), NULL);
  }

  ////////////////////////////////////// 마우스 및 카메라 처리

  void UpdatePressState(MouseButtonType type, bool flag)
  {
    m_isMousePressed = flag;
    m_buttonType     = type;
    if (!flag)
    {
      m_prevPosX = -1.f;
      m_prevPosY = -1.f;
    }
  }

  MouseButtonType WhichButtonPressed()
  {
    return m_buttonType;
  }

  void UpdateCameraOrbit(float posX, float posY)
  {
    if (!m_camPtr)
      return;
    if ((m_prevPosX < 0.f) && (m_prevPosY < 0.f))
    {
      m_prevPosX = posX;
      m_prevPosY = posY;
      return;
    }
    if (m_buttonType == MouseButtonType::LEFT)
    {
      m_camPtr->UpdateOrbit(posX - m_prevPosX, posY - m_prevPosY);
    }
    else
    {
      m_camPtr->UpdateTranslate(posX - m_prevPosX, posY - m_prevPosY);
    }
    m_prevPosX = posX;
    m_prevPosY = posY;
  }

  void UpdateCameraZoom(bool zoom)
  {
    m_camPtr->UpdateZoom(zoom);
  }

  void SetMainViewport()
  {
    // Set the viewport
    ZeroMemory(&m_mainVP, sizeof(D3D11_VIEWPORT));
    m_mainVP.TopLeftX = 0;
    m_mainVP.TopLeftY = 0;
    m_mainVP.Width    = float(m_screenOption.width);
    m_mainVP.Height   = float(m_screenOption.height);
    m_mainVP.MinDepth = 0.0f;
    m_mainVP.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &m_mainVP);
  }

  void InitPhysX()
  {
    gFoundation                      = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    gPvd                             = PxCreatePvd(*gFoundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);
    physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity                 = physx::PxVec3(0.0f, -9.81f, 0.0f);
    gDispatcher                       = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher           = gDispatcher;
    sceneDesc.filterShader            = physx::PxDefaultSimulationFilterShader;
    sceneDesc.simulationEventCallback = &this->simulationCallback;
    sceneDesc.filterShader            = contactReportFilterShader;
    gScene                            = gPhysics->createScene(sceneDesc);

    physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if (pvdClient)
    {
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    gMaterial                         = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
    gScene->addActor(*groundPlane);
    groundPlane->setName("GROUND");
  }

  void Simulate()
  {
    if (!gScene->simulate(1.0f / 60.0f))
      std::cout << "PhysX 시뮬레이션 실패" << std::endl;
    if (!gScene->fetchResults(true))
      std::cout << "PhysX 시뮬레이션 패치 실패" << std::endl;
  }

  void AddPhysxObject()
  {
  }

  // texture 처리

  MySimulationEventCallback      simulationCallback;
  physx::PxDefaultAllocator      gAllocator;
  physx::PxDefaultErrorCallback  gErrorCallback;
  physx::PxFoundation*           gFoundation = NULL;
  physx::PxPvd*                  gPvd        = NULL;
  physx::PxPhysics*              gPhysics    = NULL;
  physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
  physx::PxScene*                gScene      = NULL;
  physx::PxMaterial*             gMaterial;

public:
  void InitImGui();

  static inline void Check(HRESULT hr)
  {
    if (FAILED(hr))
    {
      throw std::exception();
    }
  }

  HWND           m_mainWindow;
  D3D11_VIEWPORT m_mainVP;

  DXGI_SWAP_CHAIN_DESC     scOption;
  D3D11_RASTERIZER_DESC    rsOption;
  D3D11_TEXTURE2D_DESC     dsOption;
  D3D11_DEPTH_STENCIL_DESC dssOption;
  D3D11_SAMPLER_DESC       samplerOption;

  Microsoft::WRL::ComPtr<ID3D11Device>            m_device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_context;
  Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_backBufferRTV;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_prevBuffer;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthTexture;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_mainDSV;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_mainRS;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_mainDepthState;
  Microsoft::WRL::ComPtr<ID3D11SamplerState>      m_mainSamplerState;

  // bool            isMousePressedFlag;
  // MouseButtonType whichButtonTypePressed;

  // CameraBuffer::SharedPtr camPtr;
  bool m_keyPressed[256] = {
    false,
  };
};