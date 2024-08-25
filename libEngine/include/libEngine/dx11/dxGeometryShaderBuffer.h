#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <Eigen/dense>
#include <dxEngine/manager/EngineManager.h>
#include <dxEngine/buffer/shader/ConstantBuffer.h>
#include <constellation/interface/GeometryShaderIF.h>

namespace dxEngine
{

template <typename GeometryContsData>
class GeometryShaderBuffer : public constellation::GeometryShaderIF
{
public:
  SHARED_PTR(GeometryShaderBuffer);

  GeometryShaderBuffer()
  {
  }

  virtual void Initialize() override
  {
    auto devPtr = EngineManager::instance().GetDevice();
    auto path   = std::wstring().assign(m_geometryShaderPath.begin(), m_geometryShaderPath.end());
    EngineManager::instance().CreateGeometryShader(devPtr, path, m_geometryShaderBuffer);
    m_geometryConstBuffer.buffuerType = ConstantBufferType::GEOMETRY;
    m_geometryConstBuffer.Initialize();
  }

  virtual void Bound() override
  {
    auto ctxPtr = EngineManager::GetContext();
    ctxPtr->GSSetShader(m_geometryShaderBuffer.Get(), 0, 0);
  }

  virtual void Unbound() override
  {
    auto ctxPtr = EngineManager::GetContext();
    ctxPtr->GSSetShader(nullptr, 0, 0);
  }

public:
  // Shader part
  Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShaderBuffer;

  // Const buffer part
  ConstantBuffer<GeometryContsData> m_geometryConstBuffer;
};
}  // namespace dxEngine