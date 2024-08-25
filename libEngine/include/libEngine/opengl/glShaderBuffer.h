#pragma once
#include <Eigen/dense>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <exception>

#include "libEngine/opengl/glConstantBuffer.h"
#include "libEngine/shared/ShaderBufferBase.h"
#include "libEngine/model/VertexShaderModel.h"
#include "libEngine/model/PixelShaderModel.h"
#include "libEngine/model/GeometryShaderModel.h"

namespace libEngine
{
template <typename VTX_C = VertexShaderModel, typename PXL_C = PixelShaderModel, typename GEOM_C = GeometryShaderModel>
class glShaderBuffer : public ShaderBufferBase<VTX_C, PXL_C, GEOM_C>
{
public:
  SHARED_PTR(glShaderBuffer)

  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  glShaderBuffer();
  ~glShaderBuffer();

  virtual void Initialize() override;
  virtual void Bound() override;
  virtual void Unbound() override;

protected:
  virtual void InitVertexConstBuffer() override;
  virtual void InitPixelConstBuffer() override;
  virtual void InitGeometryConstBuffer() override;
  virtual void InitVertexShader(std::string) override;
  virtual void InitPixelShader(std::string) override;
  virtual void InitGeometryShader(std::string) override;

  void         checkCompileErrors(GLuint shader, std::string type);
  unsigned int ID;
};
}  // namespace libEngine

#include "libEngine/opengl/glShaderBuffer.tpp"