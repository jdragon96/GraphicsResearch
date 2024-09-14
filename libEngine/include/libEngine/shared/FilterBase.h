#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <functional>

#include "libEngine/utils/macro.h"
#include "libEngine/shared/ShaderBufferBase.h"

namespace libEngine
{
template <typename VTX, typename PXL, typename GEOM>
class FilterBase
{
public:
  SHARED_PTR_ALIAS(FilterBase);

  FilterBase();
  ~FilterBase();

  virtual void Initialize() = 0;

  virtual void Render() = 0;

  virtual void SetShader(ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr);

  void IsFirstFilter()
  {
    firstFilter = true;
  }
  void LastFirstFilter()
  {
    lastFilter = true;
  }

  void SetScreenSize(float w, float h);

  ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr m_shaderBuffer;

  float m_width;
  float m_height;
  bool  firstFilter = false;
  bool  lastFilter  = false;
};
}  // namespace libEngine

#include "libEngine/shared/FilterBase.tpp"