#include "libEngine/MemDB.h"

namespace libEngine
{
EngineType              libEngine::MemDB::CurrentEngineType = EngineType::NONE;
CameraBuffer::SharedPtr libEngine::MemDB::CurrentCamera     = nullptr;
}  // namespace libEngine