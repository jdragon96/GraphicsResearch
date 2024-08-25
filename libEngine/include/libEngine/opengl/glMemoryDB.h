#pragma once
#include <Eigen/dense>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/model/NormalEffectModel.h"
#include "libEngine/utils/macro.h"

namespace libEngine
{

class glMemoryDB
{
public:
  SINGLETON_PTR(glMemoryDB)

  typedef glShaderBuffer<NormalEffect::VertexConstBuf, NormalEffect::PixelConstBuf, NormalEffect::GeometryConstBuf>
      NormalEffectShader;

  glMemoryDB()
  {
    normalEffect = NormalEffectShader::MakeShared();
    normalEffect->SetShaderCode(ShaderType::VERTEX, NormalEffect::glVertexShader);
    normalEffect->SetShaderCode(ShaderType::PIXEL, NormalEffect::glPixelShader);
    normalEffect->SetShaderCode(ShaderType::GEOMETRY, NormalEffect::glGeometryShader);
    normalEffect->Initialize();
  }

  NormalEffectShader::SharedPtr normalEffect;
};
}  // namespace libEngine