#pragma once
#include <Eigen/dense>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "libEngine/shared/ShaderBufferBase.h"

namespace libEngine
{
class glShaderBuffer : public ShaderBufferBase
{
public:
  SHARED_PTR(glShaderBuffer)

  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  glShaderBuffer();
  ~glShaderBuffer();

  virtual void UpdateMat4(std::string name, Mat4* data) override;
  virtual void UpdateVec3(std::string name, Vec3* data) override;
  virtual void UpdateVec4(std::string name, Vec4* data) override;
  virtual void Bound() override;

protected:
  virtual void InitBuffers() override;

private:
  void         checkCompileErrors(GLuint shader, std::string type);
  unsigned int ID;
};
}  // namespace libEngine