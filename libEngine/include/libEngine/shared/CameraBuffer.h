#pragma once

#include <Eigen/Dense>

#include "libEngine/utils/macro.h"
#include "libEngine/utils/eigen.h"

namespace libEngine
{
struct CameraOption
{
  float sensitivity;
  float fovy;
  float aspect;
  float zNear;
  float zFar;
  Vec3  cam;
  Vec3  target;
  Vec3  up;

  CameraOption();

  void SetPerspective(float fovy, float aspect, float zNear, float zFar);
  void SetView(Vec3 cam, Vec3 target, Vec3 up);
};

class CameraBuffer
{
public:
  SHARED_PTR(CameraBuffer);

  struct CameraParam
  {
    Eigen::Vector3f camPos;
    Eigen::Vector3f targetPos;
  };

  CameraBuffer();
  ~CameraBuffer();

  virtual void Initialize();

  void SetOption(CameraOption opt);

  virtual void SetPersepctive(float fovy, float aspect, float zNear, float zFar);

  virtual void SetRatio(float aspect);

  virtual void SetView(Vec3 cam, Vec3 target, Vec3 up);

  void SetSensitivity(float sensitivity);

  Mat4* GetViewMatPtr();

  Mat4* GetProjMatPtr();

  Vec3& GetCameraPos();

  virtual void UpdateOrbit(float deltaX, float deltaY);

  virtual void UpdateZoom(float deltaZoom);

protected:
  CameraOption m_option;

  Eigen::Vector3f m_upVector;
  Eigen::Vector3f m_frontVector;
  Eigen::Vector3f m_rightVector;
  Eigen::Vector3f m_camPosition;
  Eigen::Vector3f m_targetPosition;

  Mat4 m_viewMatrix;
  Mat4 m_projMatrix;

  float m_distance;
  float m_sensitivity;
  float m_deltaLon;  // 위도
  float m_deltaLat;  // 위도
  float m_deltaZoom;
  float m_deltaX;
  float m_deltaY;

  float m_fovy;
  float m_zNear;
  float m_zFar;
};
}  // namespace libEngine