#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <string>

#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/utils/macro.h"

#include "libEngine/dx11/dxShaderBuffer.h"
#include "libEngine/model/NormalEffectModel.h"

namespace libEngine
{

template <typename VTX_C = NormalVertexConstBuffer, typename PXL_C = NormalPixelConstBuffer,
          typename GEOM_C = NormalGeometryConstBuffer>
class dxNormalShader : public dxShaderBuffer<VTX_C, PXL_C, GEOM_C>
{
public:
  SHARED_PTR(dxNormalShader);

  dxNormalShader();
  ~dxNormalShader();

  virtual void initialize() override;

protected:
  // virtual void InitBuffers() override;
  virtual void InitVertexShader() override;
  virtual void InitPixelShader() override;
  virtual void InitGeometryShader() override;
};
}  // namespace libEngine

#include "libEngine/dx11/effect/dxNormalShader.tpp"