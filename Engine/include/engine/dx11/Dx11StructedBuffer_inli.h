#pragma once
#include "engine/dx11/Dx11StructedBuffer.h"

template <typename T>
Dx11StructedBuffer<T>::Dx11StructedBuffer()
{
}

template <typename T>
void Dx11StructedBuffer<T>::Initialize(std::vector<T>& data)
{
  auto devPtr      = Dx11EngineManager::instance().GetDevicePtr();
  auto sizeElement = sizeof(T);
  auto numElements = data.size();

  // 1. GPU buffer
  D3D11_BUFFER_DESC bufferDesc;
  ZeroMemory(&bufferDesc, sizeof(bufferDesc));
  bufferDesc.Usage               = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth           = numElements * sizeElement;
  bufferDesc.BindFlags           = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
  bufferDesc.StructureByteStride = sizeElement;
  bufferDesc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
  D3D11_SUBRESOURCE_DATA bufferData;
  ZeroMemory(&bufferData, sizeof(bufferData));
  bufferData.pSysMem = data.data();
  Dx11EngineManager::Check(devPtr->CreateBuffer(&bufferDesc, &bufferData, Dx11StructedBuffer<T>::m_gpu.GetAddressOf()));

  // 2. unordered access view
  D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
  ZeroMemory(&uavDesc, sizeof(uavDesc));
  uavDesc.Format             = DXGI_FORMAT_UNKNOWN;
  uavDesc.ViewDimension      = D3D11_UAV_DIMENSION_BUFFER;
  uavDesc.Buffer.NumElements = numElements;
  Dx11EngineManager::Check(devPtr->CreateUnorderedAccessView(Dx11StructedBuffer<T>::m_gpu.Get(), &uavDesc,
                                                             Dx11StructedBuffer<T>::m_uav.GetAddressOf()));

  // 3. shader resource view
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format               = DXGI_FORMAT_UNKNOWN;
  srvDesc.ViewDimension        = D3D11_SRV_DIMENSION_BUFFER;
  srvDesc.BufferEx.NumElements = numElements;
  Dx11EngineManager::Check(devPtr->CreateShaderResourceView(Dx11StructedBuffer<T>::m_gpu.Get(), &srvDesc,
                                                            Dx11StructedBuffer<T>::m_srv.GetAddressOf()));
  // 4. staging buffer
  D3D11_BUFFER_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.ByteWidth           = numElements * sizeElement;
  desc.Usage               = D3D11_USAGE_STAGING;
  desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
  desc.StructureByteStride = sizeElement;
  Dx11EngineManager::Check(devPtr->CreateBuffer(&desc, &bufferData, Dx11StructedBuffer<T>::m_staging.GetAddressOf()));

  Dx11StructedBuffer<T>::m_cpu = data;
}

template <typename T>
void Dx11StructedBuffer<T>::CopyFrom(std::vector<T>& data)
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  // 단, 기존 버퍼랑 크기 같이야 함
  D3D11_MAPPED_SUBRESOURCE ms;
  contextPtr->Map(Dx11StructedBuffer<T>::m_staging.Get(), NULL, D3D11_MAP_WRITE, NULL, &ms);
  memcpy(ms.pData, data.data(), data.size() * sizeof(T));
  contextPtr->Unmap(Dx11StructedBuffer<T>::m_staging.Get(), NULL);
  contextPtr->CopyResource(Dx11StructedBuffer<T>::m_gpu.Get(), Dx11StructedBuffer<T>::m_staging.Get());
  Dx11StructedBuffer<T>::m_cpu = data;
}

template <typename T>
void Dx11StructedBuffer<T>::CopyTo(std::vector<T>& dst)
{
  dst.clear();
  dst.resize(Dx11StructedBuffer<T>::m_cpu.size());
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  // GPU -> staging buffer로 데이터 복사
  contextPtr->CopyResource(Dx11StructedBuffer<T>::m_staging.Get(), Dx11StructedBuffer<T>::m_gpu.Get());
  // staging buffer에서 데이터 리드
  D3D11_MAPPED_SUBRESOURCE ms;
  contextPtr->Map(Dx11StructedBuffer<T>::m_staging.Get(), NULL, D3D11_MAP_READ, NULL, &ms);
  // CPU로 데이터 카피
  memcpy(dst.data(), ms.pData, Dx11StructedBuffer<T>::m_cpu.size() * sizeof(T));
  // 릴리즈
  contextPtr->Unmap(Dx11StructedBuffer<T>::m_staging.Get(), NULL);
}

template <typename T>
void Dx11StructedBuffer<T>::RenderVertex()
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->Draw(UINT(Dx11StructedBuffer<T>::m_cpu.size()), 0);
  Clean();
}

template <typename T>
void Dx11StructedBuffer<T>::RenderCompute(int GroupX, int GroupY, int GroupZ)
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->Dispatch(GroupX, GroupY, GroupZ);
  Clean();
}

template <typename T>
void Dx11StructedBuffer<T>::UseVertexSRV(int index)
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->VSSetShaderResources(index, 1, Dx11StructedBuffer<T>::m_srv.GetAddressOf());
}

template <typename T>
void Dx11StructedBuffer<T>::UsePixelSRV(int index)
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  contextPtr->PSSetShaderResources(index, 1, Dx11StructedBuffer<T>::m_srv.GetAddressOf());
}

template <typename T>
void Dx11StructedBuffer<T>::UseComputeUAV(int index)
{
  auto contextPtr = Dx11EngineManager::instance().GetContextPtr();
  // contextPtr->CSSetShaderResources(index, 1, Dx11StructedBuffer<T>::m_srv.GetAddressOf());
  contextPtr->CSSetUnorderedAccessViews(index, 1, Dx11StructedBuffer<T>::m_uav.GetAddressOf(), NULL);
}

template <typename T>
void Dx11StructedBuffer<T>::Clean()
{
  auto                      contextPtr = Dx11EngineManager::instance().GetContextPtr();
  ID3D11ShaderResourceView* nullSRV[12] = {
    0,
  };
  ID3D11UnorderedAccessView* nullUAV[12] = {
    0,
  };
  contextPtr->CSSetUnorderedAccessViews(0, 12, nullUAV, NULL);
  contextPtr->VSSetShaderResources(0, 12, nullSRV);
  contextPtr->PSSetShaderResources(0, 12, nullSRV);
  contextPtr->CSSetShaderResources(0, 12, nullSRV);
}