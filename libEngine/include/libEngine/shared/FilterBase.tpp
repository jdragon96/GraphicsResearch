#pragma once

#include "libEngine/shared/FilterBase.h"

namespace libEngine
{
template <typename VTX, typename PXL, typename GEOM>
FilterBase<VTX, PXL, GEOM>::FilterBase()
{
}

template <typename VTX, typename PXL, typename GEOM>
FilterBase<VTX, PXL, GEOM>::~FilterBase()
{
}

template <typename VTX, typename PXL, typename GEOM>
void FilterBase<VTX, PXL, GEOM>::SetShader(ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr shader)
{
  this->m_shaderBuffer = shader;
}

template <typename VTX, typename PXL, typename GEOM>
void FilterBase<VTX, PXL, GEOM>::SetScreenSize(float w, float h)
{
  this->m_width  = w;
  this->m_height = h;
}
}  // namespace libEngine