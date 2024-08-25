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
template <typename T>
class ConstantBuffer
{
public:
  SHARED_PTR_ALIAS(ConstantBuffer);

  ConstantBuffer();

  virtual void Initialize(ShaderType type) = 0;

  virtual void Update() = 0;

  ShaderType m_buffuerType;
  T          data;
};
}  // namespace libEngine

#include "libEngine/shared/ConstantBuffer.tpp"