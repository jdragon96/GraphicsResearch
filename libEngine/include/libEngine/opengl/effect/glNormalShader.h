#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <string>

#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/model/NormalEffectModel.h"
#include "libEngine/utils/macro.h"

namespace libEngine
{

template <typename VTX_C = NormalVertexConstBuffer, typename PXL_C = NormalPixelConstBuffer,
          typename GEOM_C = NormalGeometryConstBuffer>
class glNormalShader : public glShaderBuffer<VTX_C, PXL_C, GEOM_C>
{
public:
  SHARED_PTR(glNormalShader);

  glNormalShader();
  ~glNormalShader();
};
}  // namespace libEngine

#include "libEngine/opengl/effect/glNormalShader.tpp"