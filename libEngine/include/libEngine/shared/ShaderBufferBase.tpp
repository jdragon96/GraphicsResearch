#pragma once

#include "libEngine/shared/ShaderBufferBase.h"

namespace libEngine
{
template <typename VTX_C, typename PXL_C, typename GEOM_C>
ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::ShaderBufferBase() : useVS(false), usePS(false), useGS(false)
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::~ShaderBufferBase()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::Initialize()
{
  if (this->useVS)
  {
    InitVertexShader(m_vertexShaderCode);
    InitVertexConstBuffer();
  }
  if (this->usePS)
  {
    InitPixelShader(m_pixelShaderCode);
    InitPixelConstBuffer();
  }
  if (this->useGS)
  {
    InitGeometryShader(m_geometryShaderCode);
    InitGeometryConstBuffer();
  }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
std::string ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::GetCode(std::string path)
{
  std::filesystem::path p(path);
  if (!std::filesystem::exists(p))
    throw std::exception(MakeExceptionMsg("File", path).c_str());

  std::ifstream loader;
  loader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  loader.open(path.c_str());
  std::stringstream codeStream;
  codeStream << loader.rdbuf();
  loader.close();
  return codeStream.str();
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>

void ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::SetShaderCode(ShaderType type, std::string code)
{
  switch (type)
  {
    case ShaderType::VERTEX:
      m_vertexShaderCode = code;
      useVS              = true;
      break;
    case ShaderType::PIXEL:
      m_pixelShaderCode = code;
      usePS             = true;
      break;
    case ShaderType::GEOMETRY:
      m_geometryShaderCode = code;
      useGS                = true;
      break;
  }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::SetShaderPath(ShaderType type, std::string path)
{
  this->SetShaderCode(type, GetCode(path));
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
std::string ShaderBufferBase<VTX_C, PXL_C, GEOM_C>::MakeExceptionMsg(std::string type, std::string fileName)
{
  std::string msg;
  msg = type + " file not exists\n" + fileName;
  return msg;
}
}  // namespace libEngine