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

namespace libEngine
{
template <typename T>
class glUBO
{
public:
  SHARED_PTR(glUBO)

  glUBO();

  void Initialize();

  void Update();

  T            data;
  unsigned int m_ubo;
};
}  // namespace libEngine