#pragma once

#include "engine/Macro.h"

template <typename T>
class ConstBufferBase
{
public:
  SHARED_PTR(ConstBufferBase);

  virtual void Initialize(EConstBufferType Type) = 0;
  virtual void Update(T& data)                   = 0;
  virtual void Update()                          = 0;
  virtual void Bind()                            = 0;

  /// <summary>
  /// CPU Buffer
  /// </summary>
  T m_bufferData;

  /// <summary>
  /// Buffer Type
  /// </summary>
  EConstBufferType m_bufferType;
};
