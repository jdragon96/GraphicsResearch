#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl.h>

#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/utils/macro.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace libEngine
{
enum class ConstantBufferType
{
  PIXEL,
  VERTEX,
  GEOMETRY
};

template <typename T>
class dxConstantBuffer
{
public:
  SHARED_PTR(dxConstantBuffer);

  dxConstantBuffer();

  void Initialize(ConstantBufferType type);

  void Update();

  ConstantBufferType                   m_buffuerType;
  T                                    data;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
};
}  // namespace libEngine

#include "libEngine/dx11/dxConstantBuffer.tpp"