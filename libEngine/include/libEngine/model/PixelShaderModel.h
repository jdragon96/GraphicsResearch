#pragma once

#include "libEngine/utils/macro.h"

namespace libEngine
{
struct Material
{
public:
  Vec3  ambient;        // �ֺ���(������ �������ϰ� �����ϴ� ��)
  float ambientFactor;  // ���� ����?? ������� ��� �� ������...
  Vec3  diffuse;        // Ȯ��ݻ�(������ ���� ���� �ݻ�)
  float shininess;      // 16 bytes �����ֱ� ���� �߰�
  Vec3  specular;       // ���� �ݻ�Ǵ� ���� ����
  float specularFactor;

  Material()
  {
  }
};
class Light
{
public:
  Vec3  direction;  // ���� ���
  float dummy1;
  Vec3  position;  // ���� ����
  float dummy2;

  Light()
  {
  }
};
class PixelShaderModel
{
public:
  Vec3     camWorld;
  bool     useTexture;
  Light    light;
  Material material;

  PixelShaderModel()
  {
  }
};
}  // namespace libEngine