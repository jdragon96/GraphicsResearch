#include "libEngine/shared/ShaderBufferBase.h"

namespace libEngine
{
ShaderBufferBase::ShaderBufferBase()
{
}
ShaderBufferBase::~ShaderBufferBase()
{
}
void ShaderBufferBase::SetVertexShader(std::string path)
{
  m_vertexShaderPath = path;
}
void ShaderBufferBase::SetPixelShader(std::string path)
{
  m_pixelShaderPath = path;
}
void ShaderBufferBase::initialize()
{
  std::filesystem::path vtxShader(m_vertexShaderPath);
  std::filesystem::path pxlShader(m_pixelShaderPath);

  if (!std::filesystem::exists(vtxShader))
    throw std::exception(MakeExceptionMsg("Vertex", m_vertexShaderPath).c_str());
  if (!std::filesystem::exists(pxlShader))
    throw std::exception(MakeExceptionMsg("Pixel", m_pixelShaderPath).c_str());

  InitBuffers();
}
std::string ShaderBufferBase::MakeExceptionMsg(std::string type, std::string fileName)
{
  std::string msg;
  msg = type + " file not exists\n" + fileName;
  return msg;
}
// void ShaderBufferIF::SetVertexShader()
//{
// }
// void ShaderBufferIF::SetPixelShader()
//{
// }
}  // namespace libEngine