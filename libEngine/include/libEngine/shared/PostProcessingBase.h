#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <functional>

#include "libEngine/utils/macro.h"

namespace libEngine
{
class PostProcessingBase
{
public:
  SHARED_PTR_ALIAS(PostProcessingBase);

  // PostProcessingBase();
  //~PostProcessingBase();

  virtual void Initialize() = 0;

  virtual void Update() = 0;

  virtual void Render() = 0;
};
}  // namespace libEngine