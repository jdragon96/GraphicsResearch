#pragma once

#include "libEngine/shared/PostProcessingBase.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/opengl/glRenderer.h"
#include "libEngine/opengl/glFilter.h"
#include "libEngine/model/GaussianFilter.h"

namespace libEngine
{
class glBloomEffect : public PostProcessingBase
{
public:
  SHARED_PTR(glBloomEffect);

  using GaussianS = glShaderBuffer<GaussianFilter::VTX, GaussianFilter::PXL, GaussianFilter::GEOM>;
  using GaussianF = glFilter<GaussianFilter::VTX, GaussianFilter::PXL, GaussianFilter::GEOM>;

  glBloomEffect();
  ~glBloomEffect();

  virtual void Render() override;

  virtual void Update() override;

  virtual void Initialize() override;

  GaussianS::SharedPtr combineShader;
  GaussianS::SharedPtr thresholdShader;
  GaussianS::SharedPtr gaussianShader;

protected:
  GaussianF::SharedPtr origin;
  GaussianF::SharedPtr t_filter;
  GaussianF::SharedPtr g_filter1;
  GaussianF::SharedPtr g_filter2;
  GaussianF::SharedPtr g_filter3;
  GaussianF::SharedPtr g_filter4;
  GaussianF::SharedPtr g_filter5;
  GaussianF::SharedPtr g_filter6;
  GaussianF::SharedPtr c_filter;
};
}  // namespace libEngine