#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

#include <Eigen/Dense>

#include "libEngine/shared/MeshBufferBase.h"
#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/shared/Mesh.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/dx11/dxMesh.h"

namespace libEngine
{
class dxMeshBuffer : public MeshBufferBase
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

private:
  unsigned int m_textureBufferObject;
};

}  // namespace libEngine