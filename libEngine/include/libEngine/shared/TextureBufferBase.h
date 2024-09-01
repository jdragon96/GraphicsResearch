#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <functional>

#include "libEngine/utils/macro.h"
#include "stb_image.h"

namespace libEngine
{
class TextureBufferBase
{
public:
  SHARED_PTR_ALIAS(TextureBufferBase);

  TextureBufferBase();
  ~TextureBufferBase();

  virtual void AddDDS(std::string path)                    = 0;
  virtual void AddImage(std::string path)                  = 0;
  virtual void Bound()                                     = 0;
  virtual void AddCubemap(std::vector<std::string> pathes) = 0;

protected:
  void LoadTexture(std::string path, std::function<void(unsigned char* data, int w, int h, int c)> func);
};
}  // namespace libEngine