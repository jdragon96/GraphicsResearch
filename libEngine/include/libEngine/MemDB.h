#pragma once

#include "libEngine/utils/macro.h"
#include "libEngine/shared/CameraBuffer.h"

namespace libEngine
{
class MemDB
{
public:
  static EngineType              CurrentEngineType;
  static CameraBuffer::SharedPtr CurrentCamera;

  static void SetEngine(EngineType type)
  {
    MemDB::CurrentEngineType = type;
  }
  static void SetCamera(CameraBuffer::SharedPtr cam)
  {
    MemDB::CurrentCamera = cam;
  }
};
}  // namespace libEngine