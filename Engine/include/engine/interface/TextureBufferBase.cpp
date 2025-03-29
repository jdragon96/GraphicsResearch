#include "engine/interface/TextureBufferBase.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* TextureBufferBase::ReadImage(std::string filePath, int* x, int* y, int* channel, int req_comp)
{
  return stbi_load(filePath.c_str(), x, y, channel, req_comp);
}