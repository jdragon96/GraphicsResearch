#pragma once

#include <string>
#include <filesystem>

#include "engine/common/ResourceUtils.h"

class PipelineObjectBase
{
public:
protected:
  std::string ReadFile(std::string path)
  {
    std::filesystem::path p(path);
    if (!std::filesystem::exists(p))
      throw std::exception(ResourceUtils::NotExistMsg("File", path).c_str());

    std::ifstream loader;
    loader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    loader.open(path.c_str());
    std::stringstream codeStream;
    codeStream << loader.rdbuf();
    loader.close();
    return codeStream.str();
  }
};
