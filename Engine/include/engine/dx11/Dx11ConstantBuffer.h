#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl.h>

#include "engine/Macro.h"
#include "engine/dx11/Dx11EngineManager.h"
#include "engine/interface/ConstBufferBase.h"

template <typename T>
class Dx11ConstantBuffer : public ConstBufferBase<T>
{
public:
  SHARED_PTR(Dx11ConstantBuffer);

  Dx11ConstantBuffer();

  virtual void Initialize(EConstBufferType Type) override;

  virtual void Update(T& data) override;

  virtual void Update();

  ID3D11Buffer** GetPtr()
  {
    return m_constantBuffer.GetAddressOf();
  }

  virtual void Bind() override;

  void Bind(int bufferIndex);

  void BindAll(int bufferIndex);

  void Show();

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

  T m_bufferData;
};

#include "engine/dx11/Dx11ConstantBuffer_inli.h"