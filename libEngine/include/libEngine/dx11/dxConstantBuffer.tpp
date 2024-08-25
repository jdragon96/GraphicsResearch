#pragma once

#include "libEngine/dx11/dxConstantBuffer.h"

namespace libEngine
{
template <typename T>
dxConstantBuffer<T>::dxConstantBuffer() : ConstantBuffer<T>()
{
}

template <typename T>
void dxConstantBuffer<T>::Initialize(ShaderType type)
{
  this->m_buffuerType = type;
  // BindFlag가 D3D11_BIND_CONSTANT_BUFFER일 경우, 아래 조건을 만족해야 함
  // 만약, 16byte 배수가 아니라면 "D3D11_REQ_CONSTANT_BUFFER_ELEMENET_COUNT여야 한다.
  static_assert((sizeof(T) % 16) == 0, "ConstantBuffer 사이즈는 반드시 16 byte에 정렬되어야 합니다.");

  auto devPtr = dxRenderer::instance()->GetDevicePtr();

  D3D11_BUFFER_DESC cbDesc;
  cbDesc.ByteWidth           = sizeof(this->data);
  cbDesc.Usage               = D3D11_USAGE_DYNAMIC;
  cbDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
  cbDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
  cbDesc.MiscFlags           = 0;
  cbDesc.StructureByteStride = 0;

  // Fill in the subresource data.
  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem          = &this->data;
  InitData.SysMemPitch      = 0;
  InitData.SysMemSlicePitch = 0;
  const HRESULT hr          = devPtr->CreateBuffer(&cbDesc, &InitData, m_constantBuffer.GetAddressOf());
  if (FAILED(hr))
  {
    std::cout << "Constbuffer 생성 실패" << std::endl;
  }
}

template <typename T>
void dxConstantBuffer<T>::Update()
{
  auto ctxPtr = dxRenderer::instance()->GetContextPtr();

  D3D11_MAPPED_SUBRESOURCE ms;
  ctxPtr->Map(m_constantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
  memcpy(ms.pData, &this->data, sizeof(this->data));
  ctxPtr->Unmap(m_constantBuffer.Get(), NULL);

  switch (this->m_buffuerType)
  {
    case ShaderType::VERTEX:
      ctxPtr->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
      break;
    case ShaderType::PIXEL:
      ctxPtr->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
      break;
    case ShaderType::GEOMETRY:
      ctxPtr->GSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
      break;
  }
}
}  // namespace libEngine