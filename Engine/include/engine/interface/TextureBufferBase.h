#pragma once

#include <string>

class TextureBufferBase
{
public:
  unsigned char* ReadImage(std::string filePath, int* x, int* y, int* comp, int req_comp);
};
