#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <wrl.h>
#include <exception>
#include <vector>

#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/utils/macro.h"
#include "libEngine/model/CubemapEffect.h"

namespace libEngine
{
class CubeMapBase
{
public:
  enum class CubeTextureType
  {
    NONE,
    FILES,
    DDS
  };

  SHARED_PTR_ALIAS(CubeMapBase);

  CubeMapBase();
  ~CubeMapBase();

  // right->left->top->bottom->back-front
  virtual void LoadPictures(std::vector<std::vector<std::string>> files);

  virtual void LoadDDS(std::vector<std::string> ddsPath);

  virtual void Initialize() = 0;

  virtual void Bound() = 0;

protected:
  CubeTextureType                       m_textureType;
  std::vector<std::vector<std::string>> m_singleFiles;
  std::vector<std::string>              m_ddsList;

  ShaderBufferBase<CubemapEffect::VertexConstBuf, CubemapEffect::PixelConstBuf>::SharedPtr m_cubeShader;
};
}  // namespace libEngine