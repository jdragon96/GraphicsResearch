#pragma once
#include <libEngine/opengl/glShaderBuffer.h>

namespace libEngine
{
template <typename VTX_C, typename PXL_C, typename GEOM_C>
glShaderBuffer<VTX_C, PXL_C, GEOM_C>::glShaderBuffer() : ShaderBufferBase<VTX_C, PXL_C, GEOM_C>()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
glShaderBuffer<VTX_C, PXL_C, GEOM_C>::~glShaderBuffer()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::Bound()
{
  glUseProgram(this->ID);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::Unbound()
{
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::Initialize()
{
  this->ID = glCreateProgram();
  if (this->useVS)
  {
    InitVertexShader(this->m_vertexShaderCode);
    InitVertexConstBuffer();
  }
  if (this->usePS)
  {
    InitPixelShader(this->m_pixelShaderCode);
    InitPixelConstBuffer();
  }
  if (this->useGS)
  {
    InitGeometryShader(this->m_geometryShaderCode);
    InitGeometryConstBuffer();
  }
  glLinkProgram(this->ID);
  this->checkCompileErrors(this->ID, "PROGRAM");
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitVertexConstBuffer()
{
  auto constBuf = glConstantBuffer<VTX_C>::MakeShared();
  constBuf->SetShaderID(this->ID);
  constBuf->Initialize(ShaderType::VERTEX);
  this->vertexConstBuffer = constBuf;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitPixelConstBuffer()
{
  auto constBuf = glConstantBuffer<PXL_C>::MakeShared();
  constBuf->SetShaderID(this->ID);
  constBuf->Initialize(ShaderType::PIXEL);
  this->pixelConstBuffer = constBuf;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitGeometryConstBuffer()
{
  auto constBuf = glConstantBuffer<GEOM_C>::MakeShared();
  constBuf->SetShaderID(this->ID);
  constBuf->Initialize(ShaderType::GEOMETRY);
  this->geometryConstBuffer = constBuf;
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::checkCompileErrors(GLuint shader, std::string type)
{
  GLint  success;
  GLchar infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitVertexShader(std::string code)
{
  unsigned int shaderID;
  const char*  codeChar = code.c_str();
  shaderID              = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shaderID, 1, &codeChar, NULL);
  glCompileShader(shaderID);
  glAttachShader(this->ID, shaderID);
  glDeleteShader(shaderID);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitPixelShader(std::string code)
{
  unsigned int shaderID;
  const char*  codeChar = code.c_str();
  shaderID              = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shaderID, 1, &codeChar, NULL);
  glCompileShader(shaderID);
  glAttachShader(this->ID, shaderID);
  glDeleteShader(shaderID);
}

template <typename VTX_C, typename PXL_C, typename GEOM_C>
void glShaderBuffer<VTX_C, PXL_C, GEOM_C>::InitGeometryShader(std::string code)
{
  unsigned int shaderID;
  const char*  codeChar = code.c_str();
  shaderID              = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(shaderID, 1, &codeChar, NULL);
  glCompileShader(shaderID);
  glAttachShader(this->ID, shaderID);
  glDeleteShader(shaderID);
}
}  // namespace libEngine