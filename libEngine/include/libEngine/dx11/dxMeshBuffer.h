#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

#include <Eigen/Dense>

#include "libEngine/model/BlinnPhongEffect.h"
#include "libEngine/shared/MeshBufferBase.h"
#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/shared/Mesh.h"
#include "libEngine/dx11/dxMesh.h"
#include "libEngine/dx11/dxMemoryDB.h"

namespace libEngine
{
template <typename VTX_C = BlinnPhong::VertexShaderModel, typename PXL_C = BlinnPhong::PixelShaderModel,
          typename GEOM_C = BlinnPhong::GeometryShaderModel>
class dxMeshBuffer : public MeshBufferBase<VTX_C, PXL_C, GEOM_C>
{
public:
  SHARED_PTR(dxMeshBuffer)

  dxMeshBuffer();
  ~dxMeshBuffer();

  ////! GPU Vertex �����͸� ���� �����Ѵ�.
  // void UpdateMesh(std::vector<Mesh> mesh);

  virtual void SetMesh(std::vector<MeshData>) override;

  //! ������ �� �ؽ�ó ���۸� ����
  void SetTextureBuffer(unsigned int textureBuf);

  //! �޸� �ʱ�ȭ
  virtual void Initialize() override;

  //! ������ ����
  virtual void Render() override;

protected:
  virtual void RenderNormal();

private:
  unsigned int m_textureBufferObject;
};

}  // namespace libEngine

#include "libEngine/dx11/dxMeshBuffer.tpp"