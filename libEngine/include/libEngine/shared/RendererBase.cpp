#include "libEngine/shared/RendererBase.h"

namespace libEngine
{
RendererBase::RendererBase() : m_isMousePressed(false)
{
}
RendererBase::~RendererBase()
{
}
void RendererBase::SetPrevRenderFunc(std::function<void()> func)
{
  prevFunc = func;
}
void RendererBase::SetRenderFunc(std::function<void()> func)
{
  renderFunc = func;
}
void RendererBase::SetOption(RendererOption opt)
{
  m_option = opt;
}
void RendererBase::SetCamera(CameraBuffer::SharedPtr cam)
{
  m_camPtr = cam;
}
bool& RendererBase::IsMousePressed()
{
  return m_isMousePressed;
}
void RendererBase::UpdateMousePressStatus(bool flag, int type)
{
  this->m_isMousePressed = flag;
  this->m_buttonType     = type;
}
}  // namespace libEngine