#pragma once

#include <vector>

#include "libEngine/utils/macro.h"
#include "libEngine/shared/Mesh.h"
#include "libEngine/shared/ShaderBufferBase.h"

namespace libEngine
{
// template <typename TMesh, typename TShader>
class MeshBufferBase
{
public:
  SHARED_PTR_ALIAS(MeshBufferBase);

  MeshBufferBase();
  ~MeshBufferBase();

  virtual void SetMesh(std::vector<MeshData>) = 0;

  // virtual void SetMesh(std::vector<TMesh>);

  virtual void SetShader(ShaderBufferBase::SharedPtr shader);

  void SetMeshType(MeshType type);

  virtual void Initialize() = 0;

  virtual void Render() = 0;

  std::vector<Mesh::SharedPtr>           m_meshData;
  ShaderBufferBase::SharedPtr m_mainShader;
  Mat4                        m_modelMatrix;
  MeshType                    m_meshType;
};
}  // namespace libEngine