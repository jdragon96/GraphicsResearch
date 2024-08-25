#pragma once
#include <libEngine/dx11/dxShaderBuffer.h>

namespace libEngine
{
template <typename VTX_C, typename PXL_C, typename GEOM_C>
dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::dxShaderBuffer() : ShaderBufferBase<VTX_C, PXL_C, GEOM_C>()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::~dxShaderBuffer()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::Bound()
{
  auto contextPtr = dxRenderer::instance()->GetContextPtr();
  contextPtr->IASetInputLayout(this->inputLayout.Get());
  if (this->useVS)
    contextPtr->VSSetShader(this->vertexShader.Get(), 0, 0);
  if (this->usePS)
    contextPtr->PSSetShader(this->pixelShader.Get(), 0, 0);
  if (this->useGS)
    contextPtr->GSSetShader(this->geometryShader.Get(), 0, 0);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::Unbound()
{
  auto contextPtr = dxRenderer::instance()->GetContextPtr();
  if (ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::useGS)
    contextPtr->GSSetShader(nullptr, 0, 0);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitVertexConstBuffer()
{
  this->vertexConstBuffer = dxConstantBuffer<VTX_C>::MakeShared();
  this->vertexConstBuffer->Initialize(ShaderType::VERTEX);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitPixelConstBuffer()
{
  this->pixelConstBuffer = dxConstantBuffer<PXL_C>::MakeShared();
  this->pixelConstBuffer->Initialize(ShaderType::PIXEL);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitGeometryConstBuffer()
{
  this->geometryConstBuffer = dxConstantBuffer<GEOM_C>::MakeShared();
  this->geometryConstBuffer->Initialize(ShaderType::GEOMETRY);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitVertexShader(std::string code)
{
  auto vtxPath   = std::wstring().assign(ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::m_vertexShaderPath.begin(),
                                         ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::m_vertexShaderPath.end());
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  // 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
  HRESULT hr = D3DCompile(code.c_str(), code.length(), nullptr, nullptr, nullptr, "main", "vs_5_0", compileFlags, 0,
                          &shaderBlob, &errorBlob);

  CheckResult(hr, errorBlob.Get());

  std::vector<D3D11_INPUT_ELEMENT_DESC> inputElem = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float) * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  devicePtr->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &this->vertexShader);
  devicePtr->CreateInputLayout(inputElem.data(), UINT(inputElem.size()), shaderBlob->GetBufferPointer(),
                               shaderBlob->GetBufferSize(), &this->inputLayout);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitPixelShader(std::string code)
{
  auto pxlPath   = std::wstring().assign(ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::m_pixelShaderPath.begin(),
                                         ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::m_pixelShaderPath.end());
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  // 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
  HRESULT hr = D3DCompile(code.c_str(), code.length(), nullptr, nullptr, nullptr, "main", "ps_5_0", compileFlags, 0,
                          &shaderBlob, &errorBlob);

  CheckResult(hr, errorBlob.Get());
  devicePtr->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &this->pixelShader);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitGeometryShader(std::string code)
{
  auto path      = std::wstring().assign(ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::m_geometryShaderPath.begin(),
                                         ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::m_geometryShaderPath.end());
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

  UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
  // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
  HRESULT hr = D3DCompile(code.c_str(), code.length(), nullptr, nullptr, nullptr, "main", "gs_5_0", compileFlags, 0,
                          &shaderBlob, &errorBlob);

  CheckResult(hr, errorBlob.Get());

  devicePtr->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
                                  &this->geometryShader);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxShaderBuffer<VTX_C, PXL_C, GEOM_C>::CheckResult(HRESULT hr, ID3DBlob* errorBlob)
{
  if (FAILED(hr))
  {
    // 파일이 없을 경우
    if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
    {
      std::cout << "File not found." << std::endl;
    }

    // 에러 메시지가 있으면 출력
    if (errorBlob)
    {
      std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
    }
  }
}
}  // namespace libEngine