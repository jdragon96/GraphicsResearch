#include <libEngine/opengl/glTexture.h>

namespace libEngine
{
glTexture::glTexture()
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
void glTexture::Bound()
{
  for (auto& buffer : m_textureHash)
  {
    glActiveTexture(buffer.first);
    glBindTexture(GL_TEXTURE_2D, buffer.second);
  }
}
}  // namespace libEngine