#pragma once

#include <memory>
#include <Eigen/dense>

#define SHARED_PTR(CLASS)                                                                                              \
  using SharedPtr = std::shared_ptr<CLASS>;                                                                            \
  static SharedPtr MakeShared()                                                                                        \
  {                                                                                                                    \
    return std::make_shared<CLASS>();                                                                                  \
  };

#define SHARED_PTR_ALIAS(CLASS) using SharedPtr = std::shared_ptr<CLASS>;

#define SINGLETON(CLASS)                                                                                               \
  static CLASS& instance()                                                                                             \
  {                                                                                                                    \
    static CLASS inst;                                                                                                 \
    return inst;                                                                                                       \
  };

#define SINGLETON_PTR(CLASS)                                                                                           \
  SHARED_PTR_ALIAS(CLASS)                                                                                              \
  static CLASS::SharedPtr instance()                                                                                   \
  {                                                                                                                    \
    static CLASS::SharedPtr inst;                                                                                      \
    if (inst == nullptr)                                                                                               \
    {                                                                                                                  \
      auto ptr = std::make_shared<CLASS>();                                                                            \
      inst.swap(ptr);                                                                                                  \
    }                                                                                                                  \
    return inst;                                                                                                       \
  }
;

#define FLAG_GENERATOR(NAME)                                                                                           \
  bool NAME = false;                                                                                                   \
  void Use##NAME()                                                                                                     \
  {                                                                                                                    \
    NAME = true;                                                                                                       \
  }                                                                                                                    \
  void NotUse##NAME()                                                                                                  \
  {                                                                                                                    \
    NAME = false;                                                                                                      \
  };

using Mat4 = Eigen::Matrix<float, 4, 4, Eigen::RowMajor>;
using Vec2 = Eigen::Vector2f;
using Vec3 = Eigen::Vector3f;
using Vec4 = Eigen::Vector4f;
using Quat = Eigen::Quaternionf;

struct RendererOption
{
  int         width;
  int         height;
  std::string title;
};

enum class MouseButtonType
{
  NONE,
  LEFT,
  RIGHT,
  SCROLL,
};

enum class EConstBufferType
{
  GEOMETRY,
  PIXEL_GLOBAL,
  PIXEL,
  VERTEX_GLOBAL,
  VERTEX_MODEL,
  FILTER
};

enum class EObjectBufferType
{
  POINT,
  TRIANGLE,
};