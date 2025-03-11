#pragma once

#include <Eigen/Dense>

#include "engine/common/EigenUtils.h"
#include "engine/Macro.h"

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

  void SetReflectionFlag(bool flag);

  void SetReflection(Mat4 reflection);

  Mat4* GetViewMatPtr();

  Mat4* GetProjMatPtr();

  Mat4* GetVP();

  Vec3 GetCameraPos();

  Vec3 GetFrontVector();

  Vec3 GetUpVector();

  Vec3 GetRightVector();

  void UpdateMatrix();

  virtual void UpdateOrbit(float deltaX, float deltaY);

  virtual void UpdateZoom(bool zoom);

  virtual void UpdateTranslate(float deltaX, float deltaY);

protected:
  CameraOption m_option;

  Eigen::Vector3f m_upVector;
  Eigen::Vector3f m_frontVector;
  Eigen::Vector3f m_rightVector;
  Eigen::Vector3f m_camPosition;
  Eigen::Vector3f m_targetPosition;

  Mat4 m_reflectionMatrix;
  Mat4 m_viewMatrix;
  Mat4 m_projMatrix;
  Mat4 m_vp;

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

  bool seeReflectionWorld;
};