#pragma once

#include "libEngine/dx11/effect/dxNormalShader.h"

const std::string vtxNormalShaderCode = R"(
struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float2 texturecoord : TEXTURECOORD0;
    float3 normal : NORMAL0;
};

struct GeometryShaderInput
{
    float4 posModel : POSITION;
    float3 normal : NORMAL0;
};

GeometryShaderInput main(VertexShaderInput input)
{
    GeometryShaderInput output;
    output.posModel = float4(input.pos, 1.0);
    output.normal = input.normal;
    return output;
}
    )";

const std::string geomNormalShaderCode = R"(
struct GeometryShaderInput
{
    float4 posModel : POSITION;
    float3 normal : NORMAL0;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    uint primID : SV_PrimitiveID;
};

cbuffer GeometryConstBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    matrix invTranspose;
    float3 startColor;
    float normalLength;
    float3 endColor;
    float dummy1;
};

[maxvertexcount(2)]
void main(
  point GeometryShaderInput input[1], 
  uint primID : SV_PrimitiveID,
  inout LineStream<PixelShaderInput> outputStream)
{
    PixelShaderInput output;
    output.color = float4(startColor, 1.0);
    output.primID = primID;
    
    // 1. 노멀 원점
    float4 posWorld = mul(input[0].posModel, model);
    output.pos = mul(mul(posWorld, view), projection);
    outputStream.Append(output);
    
    // 2. 노멀 방향벡터
    output.color = float4(endColor, 1.0);
    float4 norm = normalize(mul(float4(input[0].normal, 0.0), invTranspose));
    output.pos = mul(mul(posWorld + norm * normalLength, view), projection);
    outputStream.Append(output);
    outputStream.RestartStrip();
}
    )";

const std::string pixelNormalShaderCode = R"(
struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    uint primID : SV_PrimitiveID;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return input.color;
}
    )";

namespace libEngine
{

template <typename VTX_C, typename PXL_C, typename GEOM_C>
dxNormalShader<VTX_C, PXL_C, GEOM_C>::dxNormalShader() : dxShaderBuffer<VTX_C, PXL_C, GEOM_C>()
{
  this->useVS = true;
  this->usePS = true;
  this->useGS = true;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
dxNormalShader<VTX_C, PXL_C, GEOM_C>::~dxNormalShader()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxNormalShader<VTX_C, PXL_C, GEOM_C>::initialize()
{
  this->useVS = true;
  this->usePS = true;
  this->useGS = true;
  this->InitBuffers();
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxNormalShader<VTX_C, PXL_C, GEOM_C>::InitVertexShader()
{
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  HRESULT hr = D3DCompile(vtxNormalShaderCode.c_str(), vtxNormalShaderCode.length(), nullptr, nullptr, nullptr, "main",
                          "vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

  this->CheckResult(hr, errorBlob.Get());

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
void dxNormalShader<VTX_C, PXL_C, GEOM_C>::InitPixelShader()
{
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
  UINT                             compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  HRESULT hr = D3DCompile(pixelNormalShaderCode.c_str(), pixelNormalShaderCode.length(), nullptr, nullptr, nullptr,
                          "main", "ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

  this->CheckResult(hr, errorBlob.Get());
  devicePtr->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &this->pixelShader);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void dxNormalShader<VTX_C, PXL_C, GEOM_C>::InitGeometryShader()
{
  auto devicePtr = dxRenderer::instance()->GetDevicePtr();

  Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
  Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

  UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  HRESULT hr = D3DCompile(geomNormalShaderCode.c_str(), geomNormalShaderCode.length(), nullptr, nullptr, nullptr,
                          "main", "gs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

  this->CheckResult(hr, errorBlob.Get());

  devicePtr->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL,
                                  &this->geometryShader);
}
}  // namespace libEngine