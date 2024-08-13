#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <functional>

#include "libEngine/utils/macro.h"

namespace libEngine
{
struct RendererOption
{
  int         width;
  int         height;
  std::string title;
};
class RendererBase
{
public:
  SHARED_PTR_ALIAS(RendererBase);

  RendererBase();
  ~RendererBase();

  void         SetOption(RendererOption opt);
  void         SetPrevRenderFunc(std::function<void()> func);
  void         SetRenderFunc(std::function<void()> func);
  virtual void Initialize() = 0;
  virtual void Run()                                               = 0;

protected:
  RendererOption        m_option;
  std::function<void()> prevFunc   = []() {};
  std::function<void()> renderFunc = []() {};
};
}  // namespace libEngine