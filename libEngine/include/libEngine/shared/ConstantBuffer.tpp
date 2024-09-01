#pragma once

#include "libEngine/shared/ConstantBuffer.h"

namespace libEngine
{
template <typename T>
ConstantBuffer<T>::ConstantBuffer()
{
  m_totalMemSize = sizeof(T);
  m_alignSize    = alignof(T);
  m_dataMemSize  = GetConstBufferSize(m_totalMemSize, m_alignSize);
  m_memOffset    = GetConstStartMem(m_alignSize);
}

template <typename T>
char* ConstantBuffer<T>::StartMemory()
{
  char* ptr = reinterpret_cast<char*>(&this->data);
  return ptr + m_memOffset;
}
}  // namespace libEngine