#pragma once

#include "libEngine/shared/PostProcessingBase.h"
#include "libEngine/dx11/dxShaderBuffer.h"
#include "libEngine/dx11/dxRenderer.h"
#include "libEngine/dx11/dxFilter.h"
#include "libEngine/model/GaussianFilter.h"

namespace libEngine
{
class dxBloomEffect : public PostProcessingBase
{
public:
  SHARED_PTR(dxBloomEffect);

  using GaussianS = dxShaderBuffer<GaussianFilter::VTX, GaussianFilter::PXL, GaussianFilter::GEOM>;
  using GaussianF = dxFilter<GaussianFilter::VTX, GaussianFilter::PXL, GaussianFilter::GEOM>;

  dxBloomEffect();
  ~dxBloomEffect();

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