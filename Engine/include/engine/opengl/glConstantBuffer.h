#pragma once
#include <Eigen/dense>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "engine/interface/ConstBufferBase.h"
#include "engine/opengl/glConstantBuffer.h"
#include "engine/Macro.h"

template <typename T>
class glConstantBuffer : public ConstBufferBase<T>
{
public:
  SHARED_PTR(glConstantBuffer)

  glConstantBuffer();

  virtual void Initialize(EConstBufferType Type);

  virtual void Update();

  void Bind();


  void SetShaderID(unsigned int ID);

  unsigned int m_ID;
  unsigned int m_ubo;
  GLuint       m_constbufferBindingPoint;
};

#include "engine/opengl/glConstantBuffer_inli.h"