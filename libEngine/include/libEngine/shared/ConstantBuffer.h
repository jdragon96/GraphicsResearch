#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl.h>

#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/utils/macro.h"
#include "libEngine/model/ConstBase.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace libEngine
{
template <typename T>
class ConstantBuffer
{
public:
  SHARED_PTR_ALIAS(ConstantBuffer);

  ConstantBuffer();

  virtual void Initialize(ShaderType type) = 0;

  virtual void Update() = 0;

  virtual char* StartMemory();

  ShaderType m_buffuerType;
  T          data;
  uint32_t   m_totalMemSize;
  uint32_t   m_dataMemSize;
  uint32_t   m_alignSize;
  uint32_t   m_memOffset;
};
}  // namespace libEngine

#include "libEngine/shared/ConstantBuffer.tpp"