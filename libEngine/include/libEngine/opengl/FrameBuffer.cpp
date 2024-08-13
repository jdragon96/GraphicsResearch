
// #include <glEngine/buffer/FrameBuffer.h>
//
// using namespace glEngine;
//
// FrameBuffer::FrameBuffer()
//{
// }
//
// FrameBuffer::~FrameBuffer()
//{
// }
//
// void FrameBuffer::SetWindowSize(float width, float height)
//{
//   m_width  = width;
//   m_height = height;
// }
//
////! 메모리 초기화
// void FrameBuffer::Initalize()
//{
//   if (mainShader == nullptr)
//     return;
//   EngineManager::instance().MakeFrameBuffer(m_frameBuffer, m_texttureBuffer, m_renderBuffer, m_width, m_height);
//   MakeQuad();
//   quadBuffer->SetTextureBuffer(m_texttureBuffer);
// }
// void FrameBuffer::Render()
//{
//   glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
//   glClearColor(0.f, 0.0f, 0.0f, 1.0f);
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glEnable(GL_DEPTH_TEST);
//   // https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp
//   for (auto& target : renderTargets)
//   {
//     target->Render();
//     //
//   }
//   glBindFramebuffer(GL_FRAMEBUFFER, 0);
//   glDisable(GL_DEPTH_TEST);
//   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//   glClear(GL_COLOR_BUFFER_BIT);
//   quadBuffer->Render();
// }
