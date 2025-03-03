#include "engine/common/SystemUtils.h"

void SystemUtils::PrintFalseMsg(bool flag, std::string msg, bool critical)
{
  if (!flag)
  {
    std::cout << "[SystemUtils] " << msg << std::endl;
    if (critical)
    {
      throw std::exception("예외발생!");
    }
  }
}