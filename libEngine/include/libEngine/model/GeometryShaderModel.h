#pragma once

#include "libEngine/utils/macro.h"

namespace libEngine
{
class GeometryShaderModel
{
public:
  Mat4  model;
  Mat4  view;
  Mat4  projection;
  Mat4  invTranspose;
  Vec3  normalColor;
  float normalLength;

  GeometryShaderModel()
  {
  }
};
}  // namespace libEngine