#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "engine/dx11/Dx11EngineManager.h"
#include "engine/common/ResourceUtils.h"
#include "engine/Macro.h"

// 참고: DirectX_Graphic-Samples 미니엔진
// https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/PipelineState.h

// 참고: D3D12_GRAPHICS_PIPELINE_STATE_DESC
// https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_graphics_pipeline_state_desc

// PipelineStateObject: 렌더링할 때 Context의 상태를 어떻게 설정해줄지 저장
// ComputePSO는 별도로 정의

class Dx11GraphicsPSO
{
public:
  SHARED_PTR(Dx11GraphicsPSO);

  void operator=(const Dx11GraphicsPSO& pso);
  void SetBlendFactor(const float blendFactor[4]);
  void Bind();

  void SetVertexShader(std::string path, std::vector<D3D11_INPUT_ELEMENT_DESC> elements,
                       const std::vector<D3D_SHADER_MACRO> shaderMacros);
  void SetPixelShader(std::string path);
  void SetGeometryShader(std::string path);
  void SetHullShader(std::string path);
  void SetDomainShader(std::string path);

public:
  Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_vertexShader;
  Microsoft::WRL::ComPtr<ID3D11PixelShader>    m_pixelShader;
  Microsoft::WRL::ComPtr<ID3D11HullShader>     m_hullShader;
  Microsoft::WRL::ComPtr<ID3D11DomainShader>   m_domainShader;
  Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
  Microsoft::WRL::ComPtr<ID3D11InputLayout>    m_inputLayout;

  Microsoft::WRL::ComPtr<ID3D11BlendState>        m_blendState;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_rasterizerState;

  float m_blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  UINT  m_stencilRef     = 0;

  D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

private:
  std::string ReadCode(std::string path);
  void        CheckCompile(HRESULT hr, ID3DBlob* errorBlob);
};