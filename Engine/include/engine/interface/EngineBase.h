#pragma once

#include "engine/Macro.h"
#include "engine/common/CameraBuffer.h"

class EngineBase
{
public:
  void SetScreenOption(RendererOption option)
  {
    m_screenOption = option;
  }

  bool IsPressed()
  {
    return m_isMousePressed;
  }

  std::function<void()> prevFunc           = []() {};
  std::function<void()> renderFunc         = []() {};
  std::function<void()> postProcessingFunc = []() {};
  std::function<void()> imguiFunc          = []() {};

  RendererOption m_screenOption;

protected:
  CameraBuffer::SharedPtr m_camPtr;

  /// <summary>
  /// 마우스 이벤트 처리
  /// </summary>
  MouseButtonType m_buttonType     = MouseButtonType::NONE;
  bool            m_isMousePressed = false;
  float           m_prevPosX       = -1.f;
  float           m_prevPosY       = -1.f;
};