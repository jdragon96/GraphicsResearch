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
#include "libEngine/model/VertexShaderModel.h"
#include "libEngine/model/PixelShaderModel.h"

namespace libEngine
{
class dxShaderBuffer : public ShaderBufferBase
{
public:
  SHARED_PTR(dxShaderBuffer)

  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  dxShaderBuffer();
  ~dxShaderBuffer();

  virtual void UpdateMat4(std::string name, Mat4* data) override;
  virtual void UpdateVec3(std::string name, Vec3* data) override;
  virtual void UpdateVec4(std::string name, Vec4* data) override;
  virtual void Bound() override;

protected:
  virtual void InitBuffers() override;

private:
  void CheckResult(HRESULT hr, ID3DBlob* errorBlob);
  void InitVertexShader();
  void InitPixelShader();

  // Shader part
  dxConstantBuffer<VertexShaderModel>::SharedPtr vertexConstBuffer;
  dxConstantBuffer<PixelShaderModel>::SharedPtr  pixelConstBuffer;
  std::vector<D3D11_INPUT_ELEMENT_DESC>      vertexMemoryTable;
  Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
  Microsoft::WRL::ComPtr<ID3D11InputLayout>  inputLayout;
  Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixelShader;
};
}  // namespace libEngine