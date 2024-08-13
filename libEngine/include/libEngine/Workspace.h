#pragma once

#include <memory>
#include <vector>

#include "libEngine/utils/macro.h"

#include "libEngine/shared/RendererBase.h"
#include "libEngine/shared/MeshBufferBase.h"
#include "libEngine/shared/CameraBuffer.h"

#include "libEngine/opengl/glRenderer.h"
#include "libEngine/opengl/glMeshBuffer.h"
#include "libEngine/opengl/glShaderBuffer.h"

#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/dx11/dxMeshBuffer.h"
#include "libEngine/dx11/dxShaderBuffer.h"

namespace libEngine
{
class Workspace
{
public:
  SINGLETON(Workspace);

  Workspace();

  void SetEngine(EngineType type);

  ShaderBufferBase::SharedPtr MakeShader();

  CameraBuffer::SharedPtr MakeCamera(CameraOption opt);

  RendererBase::SharedPtr MakeRenderer(RendererOption opt);

  MeshBufferBase::SharedPtr MakeMeshBuffer(std::vector<MeshData> data);

  EngineType                             m_engineType;
  std::vector<MeshBufferBase::SharedPtr> m_meshBuffer;
  RendererBase::SharedPtr                m_renderer;
};
}  // namespace libEngine