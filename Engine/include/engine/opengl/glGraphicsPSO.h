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

#include "engine/Macro.h"
#include "engine/interface/PipelineObjectBase.h"

class glGraphicsPSO : public PipelineObjectBase
{
public:
  SHARED_PTR(glGraphicsPSO)

  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  glGraphicsPSO();
  ~glGraphicsPSO();

  virtual void Initialize();
  virtual void Bind();
  virtual void UnBind();

  unsigned int ID;

  void SetVertexShader(std::string path);
  void SetFragmentShader(std::string path);

protected:
  //virtual void InitVertexConstBuffer();
  //virtual void InitPixelConstBuffer();
  //virtual void InitGeometryConstBuffer();
  //virtual void InitComputeConstBuffer();
  virtual void InitVertexShader(std::string);
  virtual void InitPixelShader(std::string);
  virtual void InitGeometryShader(std::string);
  virtual void InitComputeShader(std::string);

  void checkCompileErrors(GLuint shader, std::string type);
};