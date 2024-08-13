// #pragma once
//
// #include <functional>
// #include <memory>
// #include <string>
// #include <unordered_map>
// #include <vector>
//
//// #include <glEngine/buffer/MeshBuffer.h>
//// #include <glEngine/buffer/ShaderBuffer.hpp>
//// #include <glEngine/manager/EngineManager.h>
////
//// #include <constellation/Macro.h>
//// #include <constellation/model/Mesh.h>
//
// #include <glm/common.hpp>
// #include <glm/gtc/quaternion.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/quaternion.hpp>
//
// #include "libEngine/utils/macro.h"
//
// namespace glEngine
//{
// class FrameBuffer
//{
// public:
//  SHARED_PTR(FrameBuffer)
//
//  FrameBuffer();
//  ~FrameBuffer();
//
//  void SetWindowSize(float width, float height);
//
//  void AddTarget(MeshBuffer::SharedPtr target)
//  {
//    renderTargets.push_back(target);
//  }
//
//  void SetShader(ShaderBuffer::SharedPtr shader)
//  {
//    mainShader = shader;
//  }
//
//  void ClearTarget()
//  {
//    renderTargets.clear();
//  }
//
//  //! 메모리 초기화
//  void Initalize();
//
//  //! 랜더링 루프
//  virtual void Render();
//
// private:
//  void MakeQuad()
//  {
//    constellation::Mesh quad;
//
//    constellation::Vertex leftDown;
//    leftDown.x  = -1.f;
//    leftDown.y  = -1.f;
//    leftDown.tx = 0.f;
//    leftDown.ty = 0.f;
//
//    constellation::Vertex leftUp;
//    leftUp.x  = -1.f;
//    leftUp.y  = 1.f;
//    leftUp.tx = 0.f;
//    leftUp.ty = 1.f;
//
//    constellation::Vertex rightUp;
//    rightUp.x  = 1.f;
//    rightUp.y  = 1.f;
//    rightUp.tx = 1.f;
//    rightUp.ty = 1.f;
//
//    constellation::Vertex rigjtDown;
//    rigjtDown.x  = 1.f;
//    rigjtDown.y  = -1.f;
//    rigjtDown.tx = 1.f;
//    rigjtDown.ty = 0.f;
//
//    quad.vertexes.push_back(leftDown);
//    quad.vertexes.push_back(leftUp);
//    quad.vertexes.push_back(rightUp);
//    quad.vertexes.push_back(rigjtDown);
//
//    quad.indexes.push_back(0);
//    quad.indexes.push_back(2);
//    quad.indexes.push_back(1);
//    quad.indexes.push_back(0);
//    quad.indexes.push_back(3);
//    quad.indexes.push_back(2);
//
//    quadBuffer = MeshBuffer::MakeShared();
//    quadBuffer->SetMesh(quad);
//    quadBuffer->SetShader(mainShader);
//    quadBuffer->Initialize();
//  }
//
// public:
//  unsigned int m_frameBuffer;
//  unsigned int m_texttureBuffer;
//  unsigned int m_renderBuffer;
//
//  float m_width;
//  float m_height;
//
//  std::vector<MeshBuffer::SharedPtr> renderTargets;
//
//  ShaderBuffer::SharedPtr mainShader;
//
//  MeshBuffer::SharedPtr quadBuffer;
//};
//}  // namespace glEngine
