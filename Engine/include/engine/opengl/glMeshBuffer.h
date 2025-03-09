#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

#include <Eigen/Dense>

#include "engine/model/MeshData.h"
#include "engine/interface/MeshBufferBase.h"
#include "engine/Macro.h"

template <typename T>
class glMeshBuffer : public MeshBufferBase
{
public:
  SHARED_PTR(glMeshBuffer)

  glMeshBuffer();
  ~glMeshBuffer();

  void SetTextureBuffer(unsigned int textureBuf);

  virtual void SetMesh(MeshData<T> mesh);

  virtual void Initialize();

  virtual void Render();

protected:
  virtual void RenderNormal();

  /// <summary>
  /// Vertex Object Buffer
  /// </summary>
  unsigned int m_vertexArrayObject;

  /// <summary>
  ///
  /// </summary>
  unsigned int m_vertexBufferObject;

  /// <summary>
  /// Index Object Buffer
  /// </summary>
  unsigned int m_indexBufferObject;
};

#include "engine/opengl/glMeshBuffer_inli.h"