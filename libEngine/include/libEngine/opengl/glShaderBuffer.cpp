#include <libEngine/opengl/glShaderBuffer.h>

namespace libEngine
{
glShaderBuffer::glShaderBuffer() : ShaderBufferBase()
{
}
glShaderBuffer::~glShaderBuffer()
{
}
void glShaderBuffer::UpdateMat4(std::string name, Mat4* data)
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, data->data());
}
void glShaderBuffer::UpdateVec3(std::string name, Vec3* data)
{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, data->data());
}
void glShaderBuffer::UpdateVec4(std::string name, Vec4* data)
{
  // glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, data->data());
}
void glShaderBuffer::Bound()
{
  glUseProgram(ID);
}
void glShaderBuffer::InitBuffers()
{
  // 1. retrieve the vertex/fragment source code from filePath
  std::string   vertexCode;
  std::string   fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    vShaderFile.open(m_vertexShaderPath.c_str());
    fShaderFile.open(m_pixelShaderPath.c_str());

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode   = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure& e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
  }
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  // 2. compile shaders
  unsigned int vertex, fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");
  // delete the shaders as they're linked into our program now and no longer
  // necessery
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
void glShaderBuffer::checkCompileErrors(GLuint shader, std::string type)
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
}  // namespace libEngine