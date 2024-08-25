#pragma once

#include <Eigen/dense>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "libEngine/shared/TextureBufferBase.h"

namespace libEngine
{
class glTexture : public TextureBufferBase
{
public:
  SHARED_PTR(glTexture);

  glTexture();
  ~glTexture();

  virtual void AddImage(std::string path) override;

  virtual void Bound() override;

private:
  std::unordered_map<int, unsigned int> m_textureHash;
};
}  // namespace libEngine