#pragma once

#include <vector>

#include "engine/Macro.h"
#include "engine/model/MeshData.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "engine/dx11/Dx11EnginePSO.h"

template <typename T>
class Dx11StructedBuffer
{
public:
  SHARED_PTR(Dx11StructedBuffer);

  Dx11StructedBuffer();

  /// <summary>
  ///
  /// </summary>
  /// <param name="data"></param>
  void Initialize(std::vector<T>& data);

  /// <summary>
  ///
  /// </summary>
  /// <param name="data"></param>
  void CopyFrom(std::vector<T>& data);

  /// <summary>
  ///
  /// </summary>
  /// <param name="dst"></param>
  void CopyTo(std::vector<T>& dst);

  void UseVertexSRV(int index);
  void UsePixelSRV(int index);
  void UseComputeUAV(int index);

  void Clean();

  /// <summary>
  ///
  /// </summary>
  void RenderVertex();

  void RenderCompute(int GroupX, int GroupY, int GroupZ);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const auto GetAddressOfSRV()
  {
    return m_srv.GetAddressOf();
  }

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const auto GetAddressOfUAV()
  {
    return m_uav.GetAddressOf();
  }

  std::vector<T>                                    m_cpu;
  Microsoft::WRL::ComPtr<ID3D11Buffer>              m_gpu;
  Microsoft::WRL::ComPtr<ID3D11Buffer>              m_staging;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_srv;
  Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;
};

#include "engine/dx11/Dx11StructedBuffer_inli.h"