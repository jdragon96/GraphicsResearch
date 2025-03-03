#pragma once

#include <typeinfo>
#include <type_traits>
#include <iostream>

#include "engine/dx11/Dx11ConstantBuffer.h"

template <typename T>
Dx11ConstantBuffer<T>::Dx11ConstantBuffer()
{
}

template <typename T>
void Dx11ConstantBuffer<T>::Initialize()
{
  assert((sizeof(m_bufferData) % 16) == 0);
  auto devPtr = Dx11EngineManager::instance().GetDevicePtr();

  D3D11_BUFFER_DESC cbDesc;
  cbDesc.ByteWidth           = sizeof(m_bufferData);
  cbDesc.Usage               = D3D11_USAGE_DYNAMIC;
  cbDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
  cbDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
  cbDesc.MiscFlags           = 0;
  cbDesc.StructureByteStride = 0;

  // Fill in the subresource data.
  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem          = &m_bufferData;
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

  // switch (this->m_buffuerType)
  //{
  //   case ShaderType::VERTEX:
  //     ctxPtr->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
  //     break;
  //   case ShaderType::PIXEL:
  //     ctxPtr->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
  //     break;
  //   case ShaderType::GEOMETRY:
  //     ctxPtr->GSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
  //     break;
  //   case ShaderType::COMPUTE:
  //     ctxPtr->CSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
  //     break;
  // }
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
}