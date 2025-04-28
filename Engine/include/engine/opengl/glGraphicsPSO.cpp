#include "engine/opengl/glGraphicsPSO.h"

glGraphicsPSO::glGraphicsPSO() : PipelineObjectBase()
{
  ID = glCreateProgram();
}

glGraphicsPSO::~glGraphicsPSO()
{
  if (ID)
  {
    glDeleteProgram(ID);
  }
}

void glGraphicsPSO::Bind()
{
  glUseProgram(ID);
}

void glGraphicsPSO::UnBind()
{
}

void glGraphicsPSO::SetVertexShader(std::string path)
{
  InitVertexShader(ReadFile(path));
}
void glGraphicsPSO::SetVertexShaderCode(std::string code)
{
  InitVertexShader(code);
}

void glGraphicsPSO::SetFragmentShader(std::string path)
{
  InitPixelShader(ReadFile(path));
}
void glGraphicsPSO::SetFragmentShaderCode(std::string code)
{
  InitPixelShader(code);
}

void glGraphicsPSO::Initialize()
{
  glLinkProgram(this->ID);
  this->checkCompileErrors(this->ID, "PROGRAM");
}

void glGraphicsPSO::checkCompileErrors(GLuint shader, std::string type)
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

void glGraphicsPSO::InitVertexShader(std::string code)
{
  unsigned int shaderID;
  const char*  codeChar = code.c_str();
  shaderID              = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shaderID, 1, &codeChar, NULL);
  glCompileShader(shaderID);
  glAttachShader(this->ID, shaderID);
  glDeleteShader(shaderID);
}

void glGraphicsPSO::InitPixelShader(std::string code)
{
  unsigned int shaderID;
  const char*  codeChar = code.c_str();
  shaderID              = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shaderID, 1, &codeChar, NULL);
  glCompileShader(shaderID);
  glAttachShader(this->ID, shaderID);
  glDeleteShader(shaderID);
}

void glGraphicsPSO::InitGeometryShader(std::string code)
{
  unsigned int shaderID;
  const char*  codeChar = code.c_str();
  shaderID              = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(shaderID, 1, &codeChar, NULL);
  glCompileShader(shaderID);
  glAttachShader(this->ID, shaderID);
  glDeleteShader(shaderID);
}

void glGraphicsPSO::InitComputeShader(std::string code)
{
  unsigned int shaderID;
  const char*  codeChar = code.c_str();
  shaderID              = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(shaderID, 1, &codeChar, NULL);
  glCompileShader(shaderID);
  glAttachShader(this->ID, shaderID);
  glDeleteShader(shaderID);
}