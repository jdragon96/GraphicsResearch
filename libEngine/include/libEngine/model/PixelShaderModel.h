#pragma once

#include "libEngine/utils/macro.h"

namespace libEngine
{
struct Material
{
public:
  Vec3  ambient;        // 주변광(공간상에 광범위하게 분포하는 빛)
  float ambientFactor;  // 빛의 강도?? 어떤강도로 사용 할 것인지...
  Vec3  diffuse;        // 확산반사(균일한 양의 빛을 반사)
  float shininess;      // 16 bytes 맞춰주기 위해 추가
  Vec3  specular;       // 빛이 반사되는 현상 구현
  float specularFactor;

  Material()
  {
  }
};
class Light
{
public:
  Vec3  direction;  // 빛의 밝기
  float dummy1;
  Vec3  position;  // 빛의 방향
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