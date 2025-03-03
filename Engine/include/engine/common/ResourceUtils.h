#pragma once

#include <string>
#include <filesystem>

class ResourceUtils
{
public:
  static bool FileCheck(std::string path);

  static std::string NotExistMsg(std::string type, std::string fileName)
  {
    std::string msg;
    msg = type + " file not exists\n" + fileName;
    return msg;
  }
};