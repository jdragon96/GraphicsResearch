#pragma once
#include "libEngine/opengl/glFilter.h"

using namespace libEngine;

template <typename VTX, typename PXL, typename GEOM>
glFilter<VTX, PXL, GEOM>::glFilter()
{
}

template <typename VTX, typename PXL, typename GEOM>
glFilter<VTX, PXL, GEOM>::~glFilter()
{
}

template <typename VTX, typename PXL, typename GEOM>
void glFilter<VTX, PXL, GEOM>::SetShader(ShaderBufferBase<VTX, PXL, GEOM>::SharedPtr shader)
{
  this->m_shaderBuffer = shader;
  m_shader             = reinterpret_cast<glShaderBuffer<VTX, PXL, GEOM>*>(shader.get());
}

template <typename VTX, typename PXL, typename GEOM>
void glFilter<VTX, PXL, GEOM>::Initialize()
{
  glGenFramebuffers(1, &this->m_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, this->m_frameBuffer);
  // FrameBuffer를 생성시 반드시 하나의 texture buffer를 첨부해야함
  // 1. textureBuffer
  glGenTextures(1, &this->m_textureBuffer);
  glBindTexture(GL_TEXTURE_2D, this->m_textureBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->m_width, this->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height,
  //             0, GL_RGB,
  //             GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_textureBuffer, 0);
  // 2. renderBuffer
  glGenRenderbuffers(1, &this->m_renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, this->m_renderBuffer);
  //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, glRenderer::instance()->m_option.width,
  //                      glRenderer::instance()->m_option.height);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->m_width, this->m_height);
  // glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->m_renderBuffer);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    throw std::exception("not created frame buffer");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  this->m_meshBuffer = glMeshBuffer<VTX, PXL, GEOM>::MakeShared();
  auto mesh          = Toybox::MakeSuqare();
  mesh.vertices[0].SetTextureCoord(0, 0);
  mesh.vertices[1].SetTextureCoord(0, 1);
  mesh.vertices[2].SetTextureCoord(1, 1);
  mesh.vertices[3].SetTextureCoord(1, 0);

  this->m_meshBuffer->SetMesh({ mesh });
  this->m_meshBuffer->SetShader(this->m_shaderBuffer);
  this->m_meshBuffer->Initialize();
  this->m_targetFrameBuffer = this->m_frameBuffer;
}

template <typename VTX, typename PXL, typename GEOM>
void glFilter<VTX, PXL, GEOM>::Render()
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->m_targetFrameBuffer);
  this->m_shaderBuffer->Bound();
  glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0, 0, 0, 0);
  for (int i = 0; i < m_textures.size(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    std::stringstream stream;
    stream << "texture" << std::to_string(i);
    glUniform1i(glGetUniformLocation(this->m_shader->ID, stream.str().c_str()), i);
  }
  this->m_meshBuffer->Render();
}

template <typename VTX, typename PXL, typename GEOM>
void glFilter<VTX, PXL, GEOM>::SetTextures(std::vector<unsigned int> textures)
{
  this->m_textures = textures;
}

template <typename VTX, typename PXL, typename GEOM>
void glFilter<VTX, PXL, GEOM>::SetRenderTarget(unsigned int targetFramebuffer)
{
  this->m_targetFrameBuffer = targetFramebuffer;
}
