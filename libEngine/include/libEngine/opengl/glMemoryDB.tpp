#include <libEngine/opengl/glUBO.h>

namespace libEngine
{
template <typename T>
glUBO<T>::glUBO()
{
}

template <typename T>
void glUBO<T>::Initialize()
{
  glGenBuffers(1, &m_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_STATIC_DRAW);  // 152 바이트 메모리 할당
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template <typename T>
void glUBO<T>::Update()
{
}
}  // namespace libEngine