#include "libEngine/shared/RendererBase.h"

namespace libEngine
{
RendererBase::RendererBase()
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
}  // namespace libEngine