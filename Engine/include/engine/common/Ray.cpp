#include "engine/common/Ray.h"
#include <optional>
#include <iostream>

void Ray::UpdateRay(CameraBuffer::SharedPtr cam, Vec2 cursor, double width, double height)
{
  // 1. Screen to NDC
  const double ndcX = 2.0 * cursor.x() / width - 1.0;
  const double ndcY = 1.0 - 2.0 * cursor.y() / height;
  // 2. Clip coord(Projection coord)
  Vec4 ray_near_clip = Vec4(ndcX, ndcY, 0.0, 1.0);
  // 3. proj to view space
  // 4. veiw to world space
  Vec4 view2world = (cam->GetViewMatPtrT().inverse() * (cam->GetProjMatPtrT().inverse() * ray_near_clip));
  view2world /= view2world.w();
  // 4. Update
  Vec3 camPos       = cam->GetCameraPos();
  m_rayDirection(0) = view2world.x() - camPos.x();
  m_rayDirection(1) = view2world.y() - camPos.y();
  m_rayDirection(2) = view2world.z() - camPos.z();
  m_rayDirection.normalize();
  m_rayPos = camPos;
  std::cout << cursor.x() << ", " << cursor.y() << std::endl;
}
void Ray::Trace(MeshBufferBase<VertexData>::SharedPtr meshBuffer, bool isCCW)
{
  double shortestDistance = std::numeric_limits<double>::infinity();
  m_distance              = std::numeric_limits<float>::infinity();

  VertexData* p0;
  VertexData* p1;
  VertexData* p2;
  m_traceState = false;

  for (size_t i = 0; i < meshBuffer->m_mesh.indices.size(); i += 3)
  {
    // 1. compare distance with every triangle
    if (isCCW)
    {
      p0 = (&meshBuffer->m_mesh.vertices[meshBuffer->m_mesh.indices[i]]);
      p1 = (&meshBuffer->m_mesh.vertices[meshBuffer->m_mesh.indices[i + 1]]);
      p2 = (&meshBuffer->m_mesh.vertices[meshBuffer->m_mesh.indices[i + 2]]);
    }
    else
    {
      p0 = (&meshBuffer->m_mesh.vertices[meshBuffer->m_mesh.indices[i + 2]]);
      p1 = (&meshBuffer->m_mesh.vertices[meshBuffer->m_mesh.indices[i + 1]]);
      p2 = (&meshBuffer->m_mesh.vertices[meshBuffer->m_mesh.indices[i]]);
    }
    auto res = IntersectionTriangle(p0, p1, p2);
    if (res.has_value())
    {
      // 2. find nearest triangle
      if (res.value() < shortestDistance)
      {
        shortestDistance = res.value();
        m_traceState     = true;
      }
    }
  }

  if (m_traceState)
  {
    m_distance = shortestDistance;
  }
}
void Ray::Trace(AABB::SharedPtr aabb)
{
  m_distance   = std::numeric_limits<float>::infinity();
  m_traceState = false;

  double t_min = 0.0;  // 광선 시작점부터
  double t_max = std::numeric_limits<double>::infinity();

  // 각 축에 대해 슬랩 교차 계산
  for (int i = 0; i < 3; ++i)
  {
    if (std::abs(m_rayDirection[i]) < 1e-6)
    {
      // 방향이 축에 평행 → 원점이 AABB 범위 내인지 확인
      if (m_rayPos[i] < aabb->m_min[i] || m_rayPos[i] > aabb->m_max[i])
      {
        return;  // 범위 밖 → 교차 불가
      }
      continue;  // 범위 내 → 이 축은 항상 교차
    }

    // 슬랩 교차 파라미터 계산
    double t1 = (aabb->m_min[i] - m_rayPos[i]) / m_rayDirection[i];
    double t2 = (aabb->m_max[i] - m_rayPos[i]) / m_rayDirection[i];

    // 입구/출구 결정
    double t_near = std::min(t1, t2);
    double t_far  = std::max(t1, t2);

    // 교차 구간 갱신
    t_min = std::max(t_min, t_near);
    t_max = std::min(t_max, t_far);
  }

  // 교차 조건 확인
  if (t_min <= t_max && t_min >= 0)
  {
    m_traceState = true;
    m_distance   = static_cast<float>(t_min);
  }

  //double shortestDistance = std::numeric_limits<double>::infinity();
  //m_distance              = std::numeric_limits<float>::infinity();
  //m_traceState            = false;

  //double t_x_min = (aabb->m_min.x() - m_rayPos.x()) / m_rayDirection.x();
  //double t_x_max = (aabb->m_max.x() - m_rayPos.x()) / m_rayDirection.x();
  //double t_y_min = (aabb->m_min.y() - m_rayPos.y()) / m_rayDirection.y();
  //double t_y_max = (aabb->m_max.y() - m_rayPos.y()) / m_rayDirection.y();
  //double t_z_min = (aabb->m_min.z() - m_rayPos.z()) / m_rayDirection.z();
  //double t_z_max = (aabb->m_max.z() - m_rayPos.z()) / m_rayDirection.z();

  //auto mins = { t_x_min, t_y_min, t_z_min };
  //auto maxs = { t_x_max, t_y_max, t_z_max };

  //auto entry = std::max_element(mins.begin(), mins.end());
  //auto exit  = std::min_element(maxs.begin(), maxs.end());

  //if ((*entry > *exit) || (*entry < 0))
  //  return;

  //m_traceState = true;
  //m_distance   = *entry;
}
std::optional<double> Ray::IntersectionTriangle(VertexData* p0, VertexData* p1, VertexData* p2)
{
  if (!p0 || !p1 || !p2)
    return {};

  // 1. create edge vectors
  Vec3 edge1        = Vec3(p1->x - p0->x, p1->y - p0->y, p1->z - p0->z);
  Vec3 edge2        = Vec3(p2->x - p0->x, p2->y - p0->y, p2->z - p0->z);
  Vec3 ray_cross_e2 = m_rayDirection.cross(edge2);  // (D X (v3 - v1))

  // 2. is ray parallel with triangle?
  auto determinant = edge1.dot(ray_cross_e2);
  if (determinant < 1e-5)
    return {};
  auto inv_determinant = 1.0 / determinant;
  Vec3 S               = Vec3(m_rayPos.x() - p0->x, m_rayPos.y() - p0->y, m_rayPos.z() - p0->z);  // O - v0

  // 3. find u, (O - v1) * (D X (v3 - v1))
  double u = inv_determinant * S.dot(ray_cross_e2);
  if (u < 0 || u > 1)
    return {};

  // 4. find v, D * ((O - v1) X (v2 - v1))
  Vec3 s_cross_e1 = S.cross(edge1);
  auto v          = inv_determinant * m_rayDirection.dot(s_cross_e1);
  if (v < 0 || v > 1)
    return {};

  // 5. find t, (v3 - v1) * ((O - v1) X (v2 - v1))
  auto t = inv_determinant * edge2.dot(s_cross_e1);
  if (t > std::numeric_limits<double>::epsilon())
  {
    return t;
  }

  // nothing..
  return {};
}
bool Ray::MoveTracePos(MeshBufferBase<VertexData>::SharedPtr mesh)
{
  if (m_traceState)
  {
    Vec3 tracePos = m_rayPos + m_rayDirection * m_distance;

    mesh->m_constBuffer->m_bufferData.world(0, 3) = tracePos.x();
    mesh->m_constBuffer->m_bufferData.world(1, 3) = tracePos.y();
    mesh->m_constBuffer->m_bufferData.world(2, 3) = tracePos.z();
  }
  return m_traceState;
}