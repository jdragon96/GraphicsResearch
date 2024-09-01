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
  Vec3  ambient;        // �ֺ���(������ �������ϰ� �����ϴ� ��)
  float ambientFactor;  // ���� ����?? ������� ��� �� ������...
  Vec3  diffuse;        // Ȯ��ݻ�(������ ���� ���� �ݻ�)
  float shininess;      // 16 bytes �����ֱ� ���� �߰�
  Vec3  specular;       // ���� �ݻ�Ǵ� ���� ����
  float specularFactor;
};
struct Light
{
  Vec3  direction;  // ���� ���
  float dummy1;
  Vec3  position;  // ���� ����
  float dummy2;
};
class PixelShaderModel : public ConstBase
{
public:
  Vec3     camWorld;
  bool     useTexture;  // HLSL���� 4byte�� ó��, GLSL�� 4byte
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