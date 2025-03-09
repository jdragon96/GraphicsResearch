#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>

// #include "libEngine/utils/macro.h"
// #include "libEngine/shared/RendererBase.h"
// #include "libEngine/shared/CameraBuffer.h"

#include "engine/Macro.h"
#include "engine/interface/EngineBase.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class glEngineManager : public EngineBase
{
public:
  SINGLETON(glEngineManager);

  glEngineManager();
  ~glEngineManager();

  virtual void Initialize();
  virtual void Run();
  void         InitImGui();

  unsigned int m_frameBuffer;
  unsigned int m_textureBuffer;
  unsigned int m_renderBuffer;

private:
  void InitFrameBuffer();

protected:
  GLFWwindow* m_window;
};
