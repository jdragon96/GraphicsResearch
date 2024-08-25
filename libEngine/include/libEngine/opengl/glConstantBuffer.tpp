#pragma once

#include <libEngine/opengl/glConstantBuffer.h>

namespace libEngine
{
template <typename T>
glConstantBuffer<T>::glConstantBuffer()
{
}

template <typename T>
void glConstantBuffer<T>::Initialize(ShaderType type)
{
  this->m_buffuerType = type;
  //  assign the block binding
  this->m_constbufferBindingPoint = 0;
  switch (type)
  {
    case ShaderType::VERTEX:
      this->m_constbufferBindingPoint = 0;
      break;
    case ShaderType::PIXEL:
      this->m_constbufferBindingPoint = 1;
      break;
    case ShaderType::GEOMETRY:
      this->m_constbufferBindingPoint = 2;
      break;
  }
  // GLint m_constbufferBindingPoint = glGetUniformBlockIndex(this->m_ID, "VertexConstBuffer");
  //  1. UBO 생성
  glGenBuffers(1, &this->m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, this->m_ubo);
  // 2. UBO 메모리 할당
  glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &this->data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template <typename T>
void glConstantBuffer<T>::Update()
{
  // https://stackoverflow.com/questions/29664396/updating-uniform-buffer-objects-costs-performance
  glBindBuffer(GL_UNIFORM_BUFFER, this->m_ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &this->data);
  glBindBufferRange(GL_UNIFORM_BUFFER, this->m_constbufferBindingPoint, this->m_ubo, 0, sizeof(T));
  // glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template <typename T>
void glConstantBuffer<T>::SetShaderID(unsigned int ID)
{
  this->m_ID = ID;
}
}  // namespace libEngine