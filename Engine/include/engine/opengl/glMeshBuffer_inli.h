#pragma once

#include "engine/opengl/glMeshBuffer.h"
#include "engine/model/VertexData.h"

template <typename T>
glMeshBuffer<T>::glMeshBuffer()
{
}

template <typename T>
glMeshBuffer<T>::~glMeshBuffer()
{
  if (m_vertexArrayObject)
  {
    glDeleteVertexArrays(1, &glMeshBuffer<T>::m_vertexArrayObject);
  }
  if (m_vertexBufferObject)
  {
    glDeleteBuffers(1, &glMeshBuffer<T>::m_vertexBufferObject);
  }
  if (m_indexBufferObject)
  {
    glDeleteBuffers(1, &glMeshBuffer<T>::m_indexBufferObject);
  }
}

template <typename T>
void glMeshBuffer<T>::SetTextureBuffer(unsigned int textureBuf)
{
}

template <typename T>
void glMeshBuffer<T>::SetMesh(MeshData<T> mesh)
{
  MeshBufferBase<T>::m_mesh = mesh;
}

template <typename T>
void glMeshBuffer<T>::Initialize()
{
  glGenVertexArrays(1, &glMeshBuffer<T>::m_vertexArrayObject);
  glGenBuffers(1, &glMeshBuffer<T>::m_vertexBufferObject);
  glGenBuffers(1, &glMeshBuffer<T>::m_indexBufferObject);

  int vertexBufferSize = sizeof(VertexData) * VertexCount();
  int indexBufferSize  = sizeof(uint32_t) * IndexCount();

  glBindVertexArray(glMeshBuffer<T>::m_vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, glMeshBuffer<T>::m_vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, MeshBufferBase<T>::m_mesh.vertices.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMeshBuffer<T>::m_indexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, MeshBufferBase<T>::m_mesh.indices.data(), GL_DYNAMIC_DRAW);

  int vtxSize       = sizeof(VertexData);
  int xyzOffset     = 0;
  int rgbOffset     = sizeof(float) * 3;
  int textureOffset = sizeof(float) * 7;
  int normalOffset  = sizeof(float) * 9;

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vtxSize, (void*)xyzOffset);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vtxSize, (void*)rgbOffset);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vtxSize, (void*)textureOffset);

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vtxSize, (void*)normalOffset);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

template <typename T>
void glMeshBuffer<T>::Render()
{
  //this->m_mainShader->Bound();
  //this->m_mainShader->vertexConstBuffer->data.UpdateModel(this->m_modelMatrix);
  //this->m_mainShader->vertexConstBuffer->Update();
  // if (this->m_texture)
  //   this->m_texture->Bound();
  // for (auto& txt : glMeshBuffer<T>::m_textures)
  //{
  //  txt->Bound();
  //}
  glBindVertexArray(glMeshBuffer<T>::m_vertexArrayObject);
  //  switch (this->m_meshType)
  //  {
  //    case MeshType::TRIANGLE:
  //      glDrawElements(GL_TRIANGLES, mesh->IndexCount(), GL_UNSIGNED_INT, 0);
  //      break;
  //    case MeshType::POINT:
  //      glDrawElements(GL_POINTS, mesh->IndexCount(), GL_UNSIGNED_INT, 0);
  //      break;
  //    case MeshType::LINE:
  //      glDrawElements(GL_LINES, mesh->IndexCount(), GL_UNSIGNED_INT, 0);
  //      break;
  //    default:
  //      break;
  //  }
  //  mesh->Unbound();
  //}
  // if (this->m_renderNornal)
  //{
  //   auto normalEft                                    = glMemoryDB::instance()->normalEffect;
  //   normalEft->geometryConstBuffer->data.model        = this->m_modelMatrix;
  //   normalEft->geometryConstBuffer->data.invTranspose = this->m_mainShader->vertexConstBuffer->data.invTranspose;
  //   normalEft->geometryConstBuffer->data.projection   = this->m_mainShader->vertexConstBuffer->data.projection;
  //   normalEft->geometryConstBuffer->data.view         = this->m_mainShader->vertexConstBuffer->data.view;
  //   normalEft->geometryConstBuffer->data.startColor   = Vec3(1, 0, 0);
  //   normalEft->geometryConstBuffer->data.endColor     = Vec3(0, 0, 1);
  //   normalEft->geometryConstBuffer->data.normalLength = 0.3f;
  //   normalEft->Bound();
  //   normalEft->geometryConstBuffer->Update();
  //   for (auto& mesh : this->m_meshData)
  //   {
  //     mesh->Bound();
  //     glDrawArrays(GL_POINTS, 0, mesh->VertexCount());
  //     mesh->Unbound();
  //   }
  //   normalEft->Unbound();
  // }
}

template <typename T>
void glMeshBuffer<T>::RenderNormal()
{
}
