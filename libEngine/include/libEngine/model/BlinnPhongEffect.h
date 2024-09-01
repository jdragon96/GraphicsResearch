#pragma once

#include "libEngine/utils/macro.h"
#include "libEngine/model/ConstBase.h"

namespace libEngine
{
namespace BlinnPhong
{
///////////////////////////////////////////////// Vertex
class VertexShaderModel : public ConstBase
{
public:
  Mat4 model;
  Mat4 view;
  Mat4 projection;
  Mat4 invTranspose;

  VertexShaderModel()
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

///////////////////////////////////////////////// Pixel
struct Material
{
  Vec3  ambient;        // 주변광(공간상에 광범위하게 분포하는 빛)
  float ambientFactor;  // 빛의 강도?? 어떤강도로 사용 할 것인지...
  Vec3  diffuse;        // 확산반사(균일한 양의 빛을 반사)
  float shininess;      // 16 bytes 맞춰주기 위해 추가
  Vec3  specular;       // 빛이 반사되는 현상 구현
  float specularFactor;
};
struct Light
{
  Vec3  direction;  // 빛의 밝기
  float dummy1;
  Vec3  position;  // 빛의 방향
  float dummy2;
};
class PixelShaderModel : public ConstBase
{
public:
  Vec3     camWorld;
  bool     useTexture;  // HLSL에서 4byte로 처리, GLSL도 4byte
  Light    light;
  Material material;

  virtual void UpdateCamera(CameraBuffer::SharedPtr cam)
  {
    this->camWorld = cam->GetCameraPos();
    // switch (MemDB::CurrentEngineType)
    //{
    //   case EngineType::OPENGL:
    //     this->camWorld = cam->GetCameraPos();
    //     break;
    //   case EngineType::DX11:
    //     this->camWorld = cam->GetCameraPos().transpose();
    //     break;
    // }
  }
};

///////////////////////////////////////////////// Geometry
class GeometryShaderModel : public ConstBase
{
public:
  Mat4  model;
  Mat4  view;
  Mat4  projection;
  Mat4  invTranspose;
  Vec3  normalColor;
  float normalLength;

  GeometryShaderModel()
  {
  }
};
}  // namespace BlinnPhong
}  // namespace libEngine