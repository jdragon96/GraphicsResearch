#include "engine/common/ResourceUtils.h"
#include <filesystem>

bool ResourceUtils::FileCheck(std::string path)
{
  return std::filesystem::exists(path);
}