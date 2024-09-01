#pragma once

#include "libEngine/utils/macro.h"
#include "libEngine/shared/CameraBuffer.h"
#include "libEngine/MemDB.h"

namespace libEngine
{
uint32_t GetConstBufferSize(uint32_t memSize, uint32_t align_byte);
uint32_t GetConstStartMem(uint32_t align_byte);

class ConstBase
{
public:
  virtual void UpdateModel(Mat4 model);

  virtual void UpdateCamera(CameraBuffer::SharedPtr cam);
};
}  // namespace libEngine