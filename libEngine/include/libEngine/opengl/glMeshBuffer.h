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
#include "libEngine/opengl/glMesh.h"
#include "libEngine/opengl/glMemoryDB.h"

namespace libEngine
{
class glMeshBuffer : public MeshBufferBase
{
public:
  SHARED_PTR(glMeshBuffer)

  glMeshBuffer();
  ~glMeshBuffer();

  ////! GPU Vertex �����͸� ���� �����Ѵ�
  virtual void SetMesh(std::vector<MeshData>) override;

  //! ������ �� �ؽ�ó ���۸� ����
  void SetTextureBuffer(unsigned int textureBuf);

  //! �޸� �ʱ�ȭ
  virtual void Initialize() override;

  //! ������ ����
  virtual void Render() override;

protected:
  virtual void RenderNormal();
};

}  // namespace libEngine