#pragma once
#include <Eigen/dense>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxtk/DDSTextureLoader.h>  // 큐브맵 읽을 때 필요

#include <dxgi.h>     // DXGIFactory
#include <dxgi1_4.h>  // DXGIFactory4

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <wrl.h>

#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/dx11/dxConstantBuffer.h"
#include "libEngine/model/BlinnPhongEffect.h"

namespace libEngine
{

template <typename VTX_C = BlinnPhong::VertexShaderModel, typename PXL_C = BlinnPhong::PixelShaderModel,
          typename GEOM_C = BlinnPhong::GeometryShaderModel>
class dxShaderBuffer : public ShaderBufferBase<VTX_C, PXL_C, GEOM_C>
{
public:
  SHARED_PTR(dxShaderBuffer)

  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  dxShaderBuffer();
  ~dxShaderBuffer();

  virtual void Bound() override;
  virtual void Unbound() override;

protected:
  // GPU 메모리 할당
  virtual void InitVertexConstBuffer() override;
  virtual void InitPixelConstBuffer() override;
  virtual void InitGeometryConstBuffer() override;
  virtual void InitVertexShader(std::string) override;
  virtual void InitPixelShader(std::string) override;
  virtual void InitGeometryShader(std::string) override;

  void CheckResult(HRESULT hr, ID3DBlob* errorBlob);

  std::vector<D3D11_INPUT_ELEMENT_DESC>        vertexMemoryTable;
  Microsoft::WRL::ComPtr<ID3D11VertexShader>   vertexShader;
  Microsoft::WRL::ComPtr<ID3D11InputLayout>    inputLayout;
  Microsoft::WRL::ComPtr<ID3D11PixelShader>    pixelShader;
  Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
};
}  // namespace libEngine

#include "libEngine/dx11/dxShaderBuffer.tpp"