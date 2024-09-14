#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>

#include "libEngine/utils/macro.h"
#include "libEngine/shared/RendererBase.h"
#include "libEngine/shared/CameraBuffer.h"

namespace libEngine
{
class glRenderer : public RendererBase
{
public:
  SINGLETON_PTR(glRenderer);

  glRenderer();
  ~glRenderer();
  virtual void Initialize() override;
  virtual void Run() override;

  int& GetButtonType();

  unsigned int m_frameBuffer;
  unsigned int m_textureBuffer;
  unsigned int m_renderBuffer;

private:
  void InitFrameBuffer();

protected:
  GLFWwindow* m_window;
};
}  // namespace libEngine