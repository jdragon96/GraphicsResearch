#pragma once

#include "libEngine/utils/macro.h"

namespace libEngine
{
class VertexShaderModel
{
public:
  Mat4 model;
  Mat4 view;
  Mat4 projection;
  Mat4 invTranspose;
};
}  // namespace libEngine