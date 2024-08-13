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

  ////! GPU Vertex 데이터를 새로 갱신한다.
  // void UpdateMesh(std::vector<Mesh> mesh);

  virtual void SetMesh(std::vector<MeshData>) override;

  //! 랜더링 할 텍스처 버퍼를 선정
  void SetTextureBuffer(unsigned int textureBuf);

  //! 메모리 초기화
  virtual void Initialize() override;

  //! 랜더링 루프
  virtual void Render() override;

private:
  unsigned int m_textureBufferObject;
};

}  // namespace libEngine