#pragma once

#include <vector>

#include "engine/dx11/Dx11EngineManager.h"
//#include "research/engine/GraphicsPSO.h"
//#include "research/engine/MeshBuffer.h"
//#include "research/engine/ConstantBuffer.h"
//#include "research/engine/CameraBuffer.h"
//#include "research/engine/Drawing.h"
//#include "research/engine/FBXLoader.h"

#include "engine/Macro.h"

template <typename T_ELEMENT>
class Dx11StructuredBuffer
{
public:
  SHARED_PTR(Dx11StructuredBuffer);

  virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, const UINT numElements)
  {
    m_cpu.resize(numElements);
    Initialize(device);
  }

  virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device)
  {
    Dx11EngineManager::CreateStructuredBuffer(device, UINT(m_cpu.size()), sizeof(T_ELEMENT), m_cpu.data(), m_gpu, m_srv,
                                          m_uav);
    Dx11EngineManager::CreateStagingBuffer(device, UINT(m_cpu.size()), sizeof(T_ELEMENT), NULL, m_staging);
  }

  void Upload(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context)
  {
    Upload(context, m_cpu);
  }

  void Upload(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, std::vector<T_ELEMENT>& arrCpu)
  {
    assert(arrCpu.size() == m_cpu.size());

    Dx11EngineManager::CopyToStagingBuffer(context, m_staging, UINT(arrCpu.size() * sizeof(T_ELEMENT)), arrCpu.data());
    context->CopyResource(m_gpu.Get(), m_staging.Get());
  }

  void Download(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context)
  {
    Download(context, m_cpu);
  }

  void Download(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context, std::vector<T_ELEMENT>& arrCpu)
  {
    assert(arrCpu.size() == m_cpu.size());
    context->CopyResource(m_staging.Get(), m_gpu.Get());
    Dx11EngineManager::CopyFromStagingBuffer(context, m_staging, UINT(arrCpu.size() * sizeof(T_ELEMENT)), arrCpu.data());
  }

  const auto GetBuffer()
  {
    return m_gpu.Get();
  }
  const auto GetSRV()
  {
    return m_srv.Get();
  }
  const auto GetUAV()
  {
    return m_uav.Get();
  }
  const auto GetAddressOfSRV()
  {
    return m_srv.GetAddressOf();
  }
  const auto GetAddressOfUAV()
  {
    return m_uav.GetAddressOf();
  }
  const auto GetStaging()
  {
    return m_staging.Get();
  }

  std::vector<T_ELEMENT>               m_cpu;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_gpu;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_staging;

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_srv;
  Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;
};