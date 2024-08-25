#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <wrl.h>

#include "libEngine/utils/macro.h"
#include "libEngine/dx11/dxShaderBuffer.h"
#include "libEngine/model/NormalEffectModel.h"

namespace libEngine
{
class dxMemoryDB
{
public:
  SINGLETON_PTR(dxMemoryDB);

  typedef dxShaderBuffer<NormalEffect::VertexConstBuf, NormalEffect::PixelConstBuf, NormalEffect::GeometryConstBuf>
      NormalEffectShader;

  dxMemoryDB()
  {
    normalEffect = NormalEffectShader::MakeShared();
    normalEffect->SetShaderCode(ShaderType::VERTEX, NormalEffect::dxVertexShaader);
    normalEffect->SetShaderCode(ShaderType::PIXEL, NormalEffect::dxPixelShader);
    normalEffect->SetShaderCode(ShaderType::GEOMETRY, NormalEffect::dxGeometryShader);
    normalEffect->Initialize();
  }

  NormalEffectShader::SharedPtr normalEffect;
};
}  // namespace libEngine