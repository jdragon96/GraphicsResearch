#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl.h>

#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/utils/macro.h"
#include "libEngine/shared/ConstantBuffer.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace libEngine
{
template <typename T>
class dxConstantBuffer : public ConstantBuffer<T>
{
public:
  SHARED_PTR(dxConstantBuffer);

  dxConstantBuffer();

  virtual void Initialize(ShaderType type) override;

  virtual void Update() override;

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
};
}  // namespace libEngine

#include "libEngine/dx11/dxConstantBuffer.tpp"