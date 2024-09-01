#pragma once

#include <Eigen/dense>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <wrl.h>

#include "libEngine/opengl/glMeshBuffer.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/opengl/glTexture.h"
#include "libEngine/shared/CubeMapBase.h"
#include "libEngine/model/CubemapEffect.h"
#include "libEngine/utils/toybox.h"

namespace libEngine
{
class glCubemap : public CubeMapBase
{ 
public:
  SHARED_PTR(glCubemap);

  glCubemap();
  ~glCubemap();

  virtual void Initialize() override;

  virtual void Bound() override;

private:
  glMeshBuffer<CubemapEffect::VertexConstBuf, CubemapEffect::PixelConstBuf>::SharedPtr m_cubeMeshBuffer;
  glTexture::SharedPtr                                                                 m_texture;
};
}  // namespace libEngine