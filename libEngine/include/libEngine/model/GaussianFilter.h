/****************************************************************************************
[Fresnel 효과]
1. toEye 벡터가 normal과 각도차이가 없으면, 투과된 형상(고유 색상, fresnel 컬러)을 관측
2. 각도차이가 벌어 질 수록, 반사된 색상(빛 색상)을 관측

[고유 색상]
물: (0.02, 0.02, 0.02)
유리: (0.08, 0.08, 0.08)
금: (1, 0.71, 0.29)
****************************************************************************************/
#pragma once

#include <string>
#include "libEngine/model/ConstBase.h"
#include "libEngine/MemDB.h"

namespace libEngine
{
namespace GaussianFilter
{
class VTX : public ConstBase
{
public:
  Mat4 model;
  Mat4 view;
  Mat4 projection;
  Mat4 invTranspose;

  VTX()
  {
  }

  virtual void UpdateModel(Mat4 m) override
  {
    switch (MemDB::CurrentEngineType)
    {
      case EngineType::OPENGL:
        model        = m;
        invTranspose = m.inverse().transpose();
        break;
      case EngineType::DX11:
        model        = m.transpose();
        invTranspose = m.inverse();
        break;
    }
  }

  virtual void UpdateCamera(CameraBuffer::SharedPtr cam) override
  {
    switch (MemDB::CurrentEngineType)
    {
      case EngineType::OPENGL:
        projection = *cam->GetProjMatPtr();
        view       = *cam->GetViewMatPtr();
        break;
      case EngineType::DX11:
        projection = cam->GetProjMatPtr()->transpose();
        view       = cam->GetViewMatPtr()->transpose();
        break;
    }
  }
};

class PXL : public ConstBase
{
public:
  float dx;
  float dy;
  int   space;
  float threshold;
};

class GEOM : public ConstBase
{
public:
  Mat4 model;
};
}  // namespace GaussianFilter
}  // namespace libEngine