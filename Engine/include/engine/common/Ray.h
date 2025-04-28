#pragma once

#include <optional>

#include <Eigen/dense>
#include "engine/Macro.h"
#include "engine/interface/MeshBufferBase.h"
#include "engine/model/VertexData.h"
#include "engine/common/CameraBuffer.h"
#include "engine/common/AABB.h"

class IRay
{
public:
  virtual void Trace(MeshBufferBase<VertexData>::SharedPtr mesh, bool isCCW = true)
  {
  }
  virtual void Trace(AABB::SharedPtr aabb)
  {
  }
  virtual void UpdateRay(CameraBuffer::SharedPtr cam, Vec2 cursor, double width, double height)
  {
  }
  virtual bool MoveTracePos(MeshBufferBase<VertexData>::SharedPtr mesh)
  {
    return false;
  }

  Vec3  m_rayDirection;
  Vec3  m_rayPos;
  float m_distance   = std::numeric_limits<float>::infinity();
  bool  m_traceState = false;
};

class Ray : public IRay
{
public:
  SHARED_PTR(Ray);

  virtual void UpdateRay(CameraBuffer::SharedPtr cam, Vec2 cursor, double width, double height) override;
  virtual void Trace(MeshBufferBase<VertexData>::SharedPtr mesh, bool isCCW = true) override;
  virtual void Trace(AABB::SharedPtr aabb) override;
  virtual bool MoveTracePos(MeshBufferBase<VertexData>::SharedPtr mesh) override;

private:
  std::optional<double> IntersectionTriangle(VertexData*, VertexData*, VertexData*);
};