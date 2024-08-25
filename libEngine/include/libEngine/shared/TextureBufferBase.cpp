#define STB_IMAGE_IMPLEMENTATION
#include "libEngine/shared/TextureBufferBase.h"
#include <iostream>

namespace libEngine
{
TextureBufferBase::TextureBufferBase()
{
}
TextureBufferBase::~TextureBufferBase()
{
}
void TextureBufferBase::LoadTexture(std::string path, std::function<void(unsigned char* data, int w, int h, int c)> func)
{
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* img = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (img)
  {
    func(img, width, height, nrChannels);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(img);
}
}  // namespace libEngine