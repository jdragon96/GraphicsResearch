#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "libEngine/shared/Mesh.h"

namespace libEngine
{
class glMesh : public Mesh
{
public:
  SHARED_PTR(glMesh);

  glMesh();
  ~glMesh();
  virtual void InitBuffer() final;
  virtual void Bound() final;
  virtual void Unbound() final;

  unsigned int m_vertexArrayObject;
  unsigned int m_vertexBufferObject;
  unsigned int m_indexBufferObject;
};
}  // namespace libEngine