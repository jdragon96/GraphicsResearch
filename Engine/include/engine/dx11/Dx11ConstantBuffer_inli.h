#pragma once

#include <typeinfo>
#include <type_traits>
#include <iostream>

#include "engine/dx11/Dx11ConstantBuffer.h"

template <typename T>
Dx11ConstantBuffer<T>::Dx11ConstantBuffer() : ConstBufferBase<T>()
{
}

template <typename T>
void Dx11ConstantBuffer<T>::Initialize(EConstBufferType Type)
{
  assert((sizeof(ConstBufferBase<T>::m_bufferData) % 16) == 0);
  auto devPtr                      = Dx11EngineManager::instance().GetDevicePtr();
  ConstBufferBase<T>::m_bufferType = Type;

  D3D11_BUFFER_DESC cbDesc;
  cbDesc.ByteWidth           = sizeof(this->m_bufferData);
  cbDesc.Usage               = D3D11_USAGE_DYNAMIC;
  cbDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
  cbDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
  cbDesc.MiscFlags           = 0;
  cbDesc.StructureByteStride = 0;

  // Fill in the subresource data.
  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem          = &this->m_bufferData;
  InitData.SysMemPitch      = 0;
  InitData.SysMemSlicePitch = 0;
  const HRESULT hr          = devPtr->CreateBuffer(&cbDesc, &InitData, m_constantBuffer.GetAddressOf());
  if (FAILED(hr))
  {
    std::cout << "Constbuffer ���� ����" << std::endl;
  }
}

template <typename T>
void Dx11ConstantBuffer<T>::Update(T& data)
{
  auto ctxPtr = Dx11EngineManager::instance().GetContextPtr();

  D3D11_MAPPED_SUBRESOURCE ms;
  ctxPtr->Map(m_constantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
  memcpy(ms.pData, &data, sizeof(T));
  ctxPtr->Unmap(m_constantBuffer.Get(), NULL);
}

template <typename T>
void Dx11ConstantBuffer<T>::Update()
{
  auto ctxPtr = Dx11EngineManager::instance().GetContextPtr();

  D3D11_MAPPED_SUBRESOURCE ms;
  ctxPtr->Map(this->m_constantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
  memcpy(ms.pData, &this->m_bufferData, sizeof(T));
  ctxPtr->Unmap(this->m_constantBuffer.Get(), NULL);
}

template <typename T>
void Dx11ConstantBuffer<T>::Bind()
{
  /**
   * 0 : vertex 공통 데이터
   * 5 : pixel 공통 데이터
   * 6 : Filtering 데이터
   * 7 : Gemoetry 공통 데이터
   */
  auto ctxPtr = Dx11EngineManager::instance().GetContextPtr();
  switch (ConstBufferBase<T>::m_bufferType)
  {
    case EConstBufferType::VERTEX_GLOBAL:
      ctxPtr->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::VERTEX_MODEL:
      ctxPtr->VSSetConstantBuffers(1, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::PIXEL_GLOBAL:
      ctxPtr->PSSetConstantBuffers(2, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::COMPUTE:
      ctxPtr->CSSetConstantBuffers(3, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::PIXEL:
      ctxPtr->PSSetConstantBuffers(5, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::FILTER:
      ctxPtr->PSSetConstantBuffers(6, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::GEOMETRY:
      ctxPtr->GSSetConstantBuffers(7, 1, m_constantBuffer.GetAddressOf());
      break;
  }
}

template <typename T>
void Dx11ConstantBuffer<T>::Bind(int bufferIndex)
{
  auto ctxPtr = Dx11EngineManager::instance().GetContextPtr();
  switch (ConstBufferBase<T>::m_bufferType)
  {
    case EConstBufferType::VERTEX_GLOBAL:
      ctxPtr->VSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::VERTEX_MODEL:
      ctxPtr->VSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::PIXEL_GLOBAL:
      ctxPtr->PSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::PIXEL:
      ctxPtr->PSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::FILTER:
      ctxPtr->PSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::GEOMETRY:
      ctxPtr->GSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
      break;
    case EConstBufferType::COMPUTE:
      ctxPtr->CSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
      break;
  }
}

template <typename T>
void Dx11ConstantBuffer<T>::Show()
{
  ConstBufferBase<T>::m_bufferData.Render();
}

template <typename T>
void Dx11ConstantBuffer<T>::BindAll(int bufferIndex)
{
  auto ctxPtr = Dx11EngineManager::instance().GetContextPtr();
  ctxPtr->VSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
  ctxPtr->PSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
  ctxPtr->GSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
  ctxPtr->CSSetConstantBuffers(bufferIndex, 1, m_constantBuffer.GetAddressOf());
}