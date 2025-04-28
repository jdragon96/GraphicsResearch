#pragma once

#include "engine/opengl/glConstantBuffer.h"

template <typename T>
glConstantBuffer<T>::glConstantBuffer() : ConstBufferBase<T>()
{
}

template <typename T>
void glConstantBuffer<T>::Initialize(EConstBufferType Type)
{
  ConstBufferBase<T>::m_bufferType = type;
  this->m_constbufferBindingPoint  = 0;
  switch (Type)
  {
    case EConstBufferType::VERTEX_GLOBAL:
      this->m_constbufferBindingPoint = 0;
      break;
    case EConstBufferType::PIXEL:
      this->m_constbufferBindingPoint = 1;
      break;
    case EConstBufferType::GEOMETRY:
      this->m_constbufferBindingPoint = 2;
      break;
  }
  // GLint m_constbufferBindingPoint = glGetUniformBlockIndex(this->m_ID, "VertexConstBuffer");
  //  1. UBO 생성
  glGenBuffers(1, &m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  // 2. UBO 메모리 할당
  // glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_STREAM_DRAW);
  // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), G);
  int constBufferByte = sizeof(T);
  glBufferData(GL_UNIFORM_BUFFER, constBufferByte, NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, constBufferByte, 0);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template <typename T>
void glConstantBuffer<T>::Update()
{
  // https://stackoverflow.com/questions/29664396/updating-uniform-buffer-objects-costs-performance
  glBindBuffer(GL_UNIFORM_BUFFER, this->m_ubo);
  int MemoryStartByte = 0;
  int constBufferByte = sizeof(T);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, constBufferByte, MemoryStartByte);
  glBindBufferRange(GL_UNIFORM_BUFFER, this->m_constbufferBindingPoint, m_ubo, MemoryStartByte, constBufferByte);
}

template <typename T>
inline void glConstantBuffer<T>::Bind()
{
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
}

template <typename T>
void glConstantBuffer<T>::SetShaderID(unsigned int ID)
{
  this->m_ID = ID;
}
