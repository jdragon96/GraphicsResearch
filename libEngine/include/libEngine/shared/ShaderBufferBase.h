#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>

#include "libEngine/utils/macro.h"

namespace libEngine
{
class ShaderBufferBase
{
public:
  SHARED_PTR_ALIAS(ShaderBufferBase);

  ShaderBufferBase();
  ~ShaderBufferBase();

  void SetVertexShader(std::string path);
  void SetPixelShader(std::string path);
  void initialize();

  virtual void UpdateMat4(std::string name, Mat4* data) = 0;
  virtual void UpdateVec3(std::string name, Vec3* data) = 0;
  virtual void UpdateVec4(std::string name, Vec4* data) = 0;
  virtual void Bound()                                  = 0;

protected:
  virtual void InitBuffers() = 0;
  void         ValidationCheck();
  std::string  MakeExceptionMsg(std::string type, std::string fileName);

  std::string m_vertexShaderPath;
  std::string m_pixelShaderPath;
};
}  // namespace libEngine