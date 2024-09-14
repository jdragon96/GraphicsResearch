#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "libEngine/opengl/glMeshBuffer.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/shared/FilterBase.h"
#include "libEngine/utils/toybox.h"

namespace libEngine
{
template <typename VTX, typename PXL, typename GEOM>
class glFilter : public FilterBase<VTX, PXL, GEOM>
{
public:
  SHARED_PTR(glFilter)

  glFilter();
  ~glFilter();

  //! 메모리 초기화
  virtual void Initialize() override;

  //! 랜더링 루프
  virtual void Render() override;

  virtual void SetShader(ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr shader) override;

  void SetTextures(std::vector<unsigned int> textures);

  void SetRenderTarget(unsigned int targetFramebuffer);

public:
  unsigned int m_frameBuffer;
  unsigned int m_textureBuffer;
  unsigned int m_renderBuffer;

  glMeshBuffer<VTX, PXL, GEOM>::SharedPtr   m_meshBuffer;
  glShaderBuffer<VTX, PXL, GEOM>* m_shader;

private:
  std::vector<unsigned int> m_textures;
  unsigned int              m_targetFrameBuffer;
};
}  // namespace libEngine

#include "libEngine/opengl/glFilter.tpp"