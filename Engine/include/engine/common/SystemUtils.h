#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <exception>

class SystemUtils
{
public:
  static void PrintFalseMsg(bool flag, std::string msg, bool critical = false);
};