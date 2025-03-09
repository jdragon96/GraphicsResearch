#pragma once

#include "engine/Macro.h"

template <typename T>
class ConstBufferBase
{
public:
  virtual void Initialize(EConstBufferType Type) = 0;
  virtual void Update(T& data)                   = 0;
  virtual void Bind()                            = 0;

  EConstBufferType m_bufferType;
};
