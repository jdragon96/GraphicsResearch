#pragma once

#include <vector>

#include "engine/Macro.h"
#include "engine/model/MeshData.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "engine/dx11/Dx11EnginePSO.h"

enum class EDataType
{
  D2D,
  D3D
};

class Dx11ComputeBuffer
{
public:
  SHARED_PTR(Dx11ComputeBuffer);

  Dx11ComputeBuffer();

  void SetTexture(std::vector<ID3D11ShaderResourceView*> textures);
  void Initialize(int X, int Y, DXGI_FORMAT cellFormat = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
  void Initialize(int X, int Y, int Z, DXGI_FORMAT cellFormat = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
  void Execute(int GroupX, int GroupY, int GroupZ);
  void CopyTo(Microsoft::WRL::ComPtr<ID3D11Texture2D> copyTarget);
  void CopyTo(Microsoft::WRL::ComPtr<ID3D11Texture3D> copyTarget);
  void Barrier();

  std::vector<ID3D11ShaderResourceView*>            m_srv;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>           m_2dTexture;
  Microsoft::WRL::ComPtr<ID3D11Texture3D>           m_3dTexture;
  Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
  EDataType                                         m_Type;
  int                                               m_height;
  int                                               m_width;
  DXGI_FORMAT                                       m_format;
};