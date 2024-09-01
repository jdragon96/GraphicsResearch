#include <libEngine/opengl/glTexture.h>

namespace libEngine
{
glTexture::glTexture() : TextureBufferBase()
{
}
glTexture::~glTexture()
{
}
void glTexture::AddImage(std::string path)
{
  LoadTexture(path, [this](unsigned char* data, int w, int h, int c) {
    unsigned int textureBuffer;
    glGenTextures(1, &textureBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // save texture buffer
    this->m_textureHash[GL_TEXTURE0 + m_textureHash.size()] = textureBuffer;
  });
}
void glTexture::AddCubemap(std::vector<std::string> folder)
{
  if (folder.size() != 6)
    return;

  int          index = 0;
  unsigned int textureBuffer;
  glGenTextures(1, &textureBuffer);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureBuffer);
  for (auto& path : folder)
  {
    LoadTexture(path, [this, index](unsigned char* data, int w, int h, int c) {
      GLenum format;
      if (c == 1)
        format = GL_RED;
      else if (c == 3)
        format = GL_RGB;
      else if (c == 4)
        format = GL_RGBA;
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
      // glGenerateMipmap(GL_TEXTURE_2D);
      //  save texture buffer
    });
    index++;
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  this->m_textureHash[GL_TEXTURE0 + m_textureHash.size()] = textureBuffer;
}
void glTexture::Bound()
{
  for (auto& buffer : m_textureHash)
  {
    glActiveTexture(buffer.first);
    glBindTexture(GL_TEXTURE_2D, buffer.second);
  }
}
}  // namespace libEngine