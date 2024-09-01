#pragma once

#include <vector>

#include "libEngine/utils/macro.h"
#include "libEngine/shared/Mesh.h"
#include "libEngine/model/BlinnPhongEffect.h"
#include "libEngine/shared/TextureBufferBase.h"
#include "libEngine/shared/ShaderBufferBase.h"

namespace libEngine
{
template <typename VTX_C = BlinnPhong::VertexShaderModel, typename PXL_C = BlinnPhong::PixelShaderModel,
          typename GEOM_C = BlinnPhong::GeometryShaderModel>
class MeshBufferBase
{
public:
  SHARED_PTR_ALIAS(MeshBufferBase);

  MeshBufferBase();

  ~MeshBufferBase();

  virtual void Move(Vec3 pos);

  virtual void SetNormalRenderFlag(bool);

  virtual void SetModel(Mat4 model);

  virtual void SetTexture(TextureBufferBase::SharedPtr texture);

  virtual void SetMesh(std::vector<MeshData>) = 0;

  virtual void SetShader(ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::SharedPtr shader);

  void SetMeshType(MeshType type);

  virtual void Initialize() = 0;

  virtual void Render() = 0;

protected:
  virtual void RenderNormal() = 0;

  bool                                              m_renderNornal;
  std::vector<Mesh::SharedPtr>                      m_meshData;
  TextureBufferBase::SharedPtr                      m_texture;
  ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::SharedPtr m_mainShader;
  Mat4                                              m_modelMatrix;
  MeshType                                          m_meshType;
};
}  // namespace libEngine

#include "libEngine/shared/MeshBufferBase.tpp"