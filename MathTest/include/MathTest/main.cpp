#include <iostream>

// #include "libEngine/libEngine.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <Eigen/Dense>

// vcpkg install directxtk:x64-windows
#include <d3d11.h>
#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>

class model
{
public:
  std::string name;

  model()
  {
    name = "Test Name!!!";
  }
};

template <typename T = model>
class TestClass
{
public:
  TestClass()
  {
    m_model = T();
  }

  void printName();

  T m_model;
};

template <typename T = model>
class TestClass2 : public TestClass<T>
{
public:
  TestClass2()
  {
    m_model = T();
  }

  void printName2()
  {
    TestClass<T>::printName();
  }

  T m_model;
};

template <typename T>
void TestClass<T>::printName()
{
  std::cout << m_model.name << std::endl;
}

std::ostream& operator<<(std::ostream& os, DirectX::XMFLOAT4X4 m)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      os << m.m[i][j] << "\t";
    }
    os << std::endl;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, DirectX::SimpleMath::Matrix mat)
{
  os << mat._11 << ", " << mat._12 << ", " << mat._13 << ", " << mat._14 << std::endl;
  os << mat._21 << ", " << mat._22 << ", " << mat._23 << ", " << mat._24 << std::endl;
  os << mat._31 << ", " << mat._32 << ", " << mat._33 << ", " << mat._34 << std::endl;
  os << mat._41 << ", " << mat._42 << ", " << mat._43 << ", " << mat._44 << std::endl;

  return os;
}

int main()
{
  // glm: column major ¹æ½Ä
  // mat4x4(
  //   (90.000000, 100.000000, 110.000000, 120.000000),
  //   (202.000000, 228.000000, 254.000000, 280.000000),
  //   (314.000000, 356.000000, 398.000000, 440.000000),
  //   (426.000000, 484.000000, 542.000000, 600.000000))
  glm::mat4 m1 = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  std::cout << glm::to_string(m1 * m1) << std::endl;

  DirectX::XMFLOAT4X4 mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

  auto a = TestClass2<>();
  a.printName2();

  // 90, 100, 110, 120
  // 202, 228, 254, 280
  // 314, 356, 398, 440
  // 426, 484, 542, 600
  DirectX::SimpleMath::Matrix simpleMat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  std::cout << simpleMat * simpleMat << std::endl;

  return 0;
}