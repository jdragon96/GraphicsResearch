#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

#include "libEngine/shared/Mesh.h"
#include "libEngine/dx11/dxRenderer.h"

namespace libEngine
{
class dxMesh : public Mesh
{
public:
  SHARED_PTR(dxMesh);

  dxMesh();
  ~dxMesh();
  virtual void InitBuffer() final;
  virtual void Bound() final;
  virtual void Unbound() final;

  Microsoft::WRL::ComPtr<ID3D11Buffer>        m_vertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer>        m_indexBuffer;
  // TextureBuffer::SharedPtr             m_textureBuffer;
  // ShaderPtr                            m_shaderBuffer;
};
}  // namespace libEngine