#pragma once
#include <Eigen/dense>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "libEngine/shared/ConstantBuffer.h"

namespace libEngine
{
template <typename T>
class glConstantBuffer : public ConstantBuffer<T>
{
public:
  SHARED_PTR(glConstantBuffer)

  glConstantBuffer();

  virtual void Initialize(ShaderType type) override;

  virtual void Update() override;

  unsigned int m_ubo;

  void SetShaderID(unsigned int ID);

  unsigned int m_ID;
  GLuint       m_constbufferBindingPoint;
};
}  // namespace libEngine

#include "libEngine/opengl/glConstantBuffer.tpp"