#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include "libEngine/model/BlinnPhongEffect.h"
#include "libEngine/utils/macro.h"
#include "libEngine/shared/ConstantBuffer.h"

namespace libEngine
{

template <typename VTX_C = BlinnPhong::VertexShaderModel, typename PXL_C = BlinnPhong::PixelShaderModel,
          typename GEOM_C = BlinnPhong::GeometryShaderModel>
class ShaderBufferBase
{
public:
  SHARED_PTR_ALIAS(ShaderBufferBase);

  ShaderBufferBase();
  ~ShaderBufferBase();

  void SetShaderPath(ShaderType, std::string path);
  void SetShaderCode(ShaderType, std::string code);

  // 셰이더 리소스(파일) 유효성 검토
  virtual void Initialize();
  virtual void Bound()   = 0;
  virtual void Unbound() = 0;

  ConstantBuffer<VTX_C>::SharedPtr  vertexConstBuffer;
  ConstantBuffer<PXL_C>::SharedPtr  pixelConstBuffer;
  ConstantBuffer<GEOM_C>::SharedPtr geometryConstBuffer;

protected:
  // GPU 메모리 할당
  virtual void InitVertexConstBuffer()         = 0;
  virtual void InitPixelConstBuffer()          = 0;
  virtual void InitGeometryConstBuffer()       = 0;
  virtual void InitVertexShader(std::string)   = 0;
  virtual void InitPixelShader(std::string)    = 0;
  virtual void InitGeometryShader(std::string) = 0;

  std::string MakeExceptionMsg(std::string type, std::string fileName);
  std::string GetCode(std::string path);

  // @deprecated
  std::string m_vertexShaderPath;
  // @deprecateds
  std::string m_pixelShaderPath;
  // @deprecated
  std::string m_geometryShaderPath;

  std::string m_vertexShaderCode;
  std::string m_pixelShaderCode;
  std::string m_geometryShaderCode;

  bool useVS;
  bool usePS;
  bool useGS;
};
}  // namespace libEngine

#include "libEngine/shared/ShaderBufferBase.tpp"
