#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "libEngine/model/PixelShaderModel.h"
#include "libEngine/model/VertexShaderModel.h"

#include "libEngine/opengl/glMeshBuffer.h"
#include "libEngine/opengl/glRenderer.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/opengl/glTexture.h"

#include "libEngine/shared/CameraBuffer.h"
#include "libEngine/shared/Mesh.h"
#include "libEngine/shared/Vertex.h"
#include "libEngine/utils/eigen.h"
#include "libEngine/utils/toybox.h"

#include "libEngine/utils/macro.h"

namespace libEngine
{
class MemmoryDB
{
public:
  SINGLETON_PTR(MemmoryDB);
  MemmoryDB()
  {
  }
};
}  // namespace libEngine