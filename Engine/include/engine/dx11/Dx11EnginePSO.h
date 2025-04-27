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
#include "engine/interface/PipelineObjectBase.h"

class Dx11EnginePSO : public PipelineObjectBase
{
public:
  SINGLETON(Dx11EnginePSO);

  Dx11EnginePSO();

  /// <summary>
  /// Rasterization 반시계 방향
  /// </summary>
  void DrawCounterClock();

  /// <summary>
  /// Rasterization 시계
  /// </summary>
  void DrawClock();

  void ClearDepth();
  void ClearStencil();
  void ClearDepthStencil();

  void EnableStencil();
  void EnableDepth();
  void WriteDpethAndStencil();
  void EnableStencilMask();

  void EnableSampler();

  void EnableLinearSampler(int index);

  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDefaultDSV();

private:
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_solidRS;
  Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_solidCCWRS;  // Counter-ClockWise

  Microsoft::WRL::ComPtr<ID3D11BlendState>       m_mirrorBS;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
  Microsoft::WRL::ComPtr<ID3D11Texture2D>        m_depthTexture;

  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_drawDSS;        // 일반적으로 그리기
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_maskDSS;        // 스텐실버퍼에 표시
  Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_drawMaskedDSS;  // 스텐실 표시된 곳만

  // Sampler
  Microsoft::WRL::ComPtr<ID3D11SamplerState> m_linearSampler;  // 선형 보간용 샘플러
  Microsoft::WRL::ComPtr<ID3D11SamplerState> m_clampSampler;   // 클램프용 샘플러
};