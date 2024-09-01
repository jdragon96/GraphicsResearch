#include "libEngine/model/ConstBase.h"

namespace libEngine
{
uint32_t GetConstBufferSize(uint32_t memSize, uint32_t align_byte)
{
  return memSize - sizeof(ConstBase) - (sizeof(ConstBase) % align_byte);
}

uint32_t GetConstStartMem(uint32_t align_byte)
{
  return sizeof(ConstBase) + (sizeof(ConstBase) % align_byte);
}
void ConstBase::UpdateModel(Mat4 model)
{
}

void ConstBase::UpdateCamera(CameraBuffer::SharedPtr cam)
{
}

}  // namespace libEngine