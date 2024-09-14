#include "libEngine/dx11/postprocessing/dxBloomEffect.h"

namespace libEngine
{
dxBloomEffect::dxBloomEffect()
{
}
dxBloomEffect ::~dxBloomEffect()
{
}
void dxBloomEffect::Initialize()
{
  const std::string vertexShader = R"(
struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float2 texturecoord : TEXTURECOORD0;
    float3 normal : NORMAL0;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

cbuffer VertexConstBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    matrix invTranspose;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    output.position = float4(input.pos, 1.0);
    output.texcoord = input.texturecoord;
    return output;
}
)";

  thresholdShader = GaussianS::MakeShared();
  thresholdShader->SetShaderCode(ShaderType::VERTEX, vertexShader);
  thresholdShader->SetShaderCode(ShaderType::PIXEL, R"(
Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

cbuffer SamplingPixelConstantData : register(b0)
{
    float dx;
    float dy;
    int space;
    float threshold;
};

struct SamplingPixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(SamplingPixelShaderInput input) : SV_TARGET
{
    float3 color = g_texture0.Sample(g_sampler, input.texcoord).rgb;
    float l = (color.x + color.y + color.y) / 3;
    return l > threshold ? float4(color, 1.0f) : float4(0.0f, 0.0f, 0.0f, 0.0f);
}
)");
  thresholdShader->Initialize();

  combineShader = GaussianS::MakeShared();
  combineShader->SetShaderCode(ShaderType::VERTEX, vertexShader);
  combineShader->SetShaderCode(ShaderType::PIXEL, R"(
Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
SamplerState g_sampler : register(s0);

cbuffer SamplingPixelConstantData : register(b0)
{
    float dx;
    float dy;
    int space;
    float threshold;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return g_texture0.Sample(g_sampler, input.texcoord) + g_texture1.Sample(g_sampler, input.texcoord);
}
)");
  combineShader->Initialize();

  gaussianShader = GaussianS::MakeShared();
  gaussianShader->SetShaderCode(ShaderType::VERTEX, vertexShader);
  gaussianShader->SetShaderCode(ShaderType::PIXEL, R"(
Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

static const float weights[7] = { 0.05, 0.1, 0.14, 0.6, 0.14, 0.1, 0.05};
// static const float weights[5] = { 0.0, 0.0, 0.0, 0.2442, 0.8026 };

cbuffer SamplingPixelConstantData : register(b0)
{
    float dx;
    float dy;
    int space;
    float threshold;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 color = float3(0.0, 0.0, 0.0);
    int i;
    for(i = 0; i < 7; ++i)
    {
      color += weights[i] *
        g_texture0.Sample(g_sampler, input.texcoord + float2(dx , 0.0) * float((i - 3) * space)).rgb;
      color += weights[i] *
        g_texture0.Sample(g_sampler, input.texcoord + float2(0.0, dy) * float((i - 3) * space)).rgb;
    }
    return float4(color, 1.0);
}
)");
  gaussianShader->Initialize();

  auto w = dxRenderer::instance()->m_option.width;
  auto h = dxRenderer::instance()->m_option.height;

  origin = GaussianF::MakeShared();
  origin->SetShader(thresholdShader);
  origin->SetScreenSize(w, h);
  origin->Initialize();
  t_filter = GaussianF::MakeShared();
  t_filter->SetShader(thresholdShader);
  t_filter->SetScreenSize(w, h);
  t_filter->Initialize();
  g_filter1 = GaussianF::MakeShared();
  g_filter1->SetShader(gaussianShader);
  g_filter1->SetScreenSize(w, h);
  g_filter1->Initialize();
  g_filter2 = GaussianF::MakeShared();
  g_filter2->SetShader(gaussianShader);
  g_filter2->SetScreenSize(w, h);
  g_filter2->Initialize();
  g_filter3 = GaussianF::MakeShared();
  g_filter3->SetShader(gaussianShader);
  g_filter3->SetScreenSize(w, h);
  g_filter3->Initialize();
  g_filter4 = GaussianF::MakeShared();
  g_filter4->SetShader(gaussianShader);
  g_filter4->SetScreenSize(w, h);
  g_filter4->Initialize();
  g_filter5 = GaussianF::MakeShared();
  g_filter5->SetShader(gaussianShader);
  g_filter5->SetScreenSize(w, h);
  g_filter5->Initialize();
  g_filter6 = GaussianF::MakeShared();
  g_filter6->SetShader(gaussianShader);
  g_filter6->SetScreenSize(w, h);
  g_filter6->Initialize();
  c_filter = GaussianF::MakeShared();
  c_filter->SetShader(combineShader);
  c_filter->SetScreenSize(w, h);
  c_filter->Initialize();

  origin->SetShaderResources({ dxRenderer::instance()->m_mainShaderResource });
  t_filter->SetShaderResources({ dxRenderer::instance()->m_mainShaderResource });
  g_filter1->SetShaderResources({ t_filter->m_shaderResourceView });
  g_filter2->SetShaderResources({ g_filter1->m_shaderResourceView });
  g_filter3->SetShaderResources({ g_filter2->m_shaderResourceView });
  g_filter4->SetShaderResources({ g_filter3->m_shaderResourceView });
  g_filter5->SetShaderResources({ g_filter4->m_shaderResourceView });
  g_filter6->SetShaderResources({ g_filter5->m_shaderResourceView });
  c_filter->SetShaderResources({ g_filter6->m_shaderResourceView, origin->m_shaderResourceView });
  c_filter->SetRenderTargets({ dxRenderer::instance()->m_mainRenderTarget });

  thresholdShader->pixelConstBuffer->data.dx        = 1.f / w;
  thresholdShader->pixelConstBuffer->data.dy        = 1.f / h;
  thresholdShader->pixelConstBuffer->data.space     = 1;
  thresholdShader->pixelConstBuffer->data.threshold = 0.8f;

  gaussianShader->pixelConstBuffer->data.dx        = 1.f / w;
  gaussianShader->pixelConstBuffer->data.dy        = 1.f / h;
  gaussianShader->pixelConstBuffer->data.space     = 3;
  gaussianShader->pixelConstBuffer->data.threshold = 0.8f;

  combineShader->pixelConstBuffer->data.dx        = 1.f / w;
  combineShader->pixelConstBuffer->data.dy        = 1.f / h;
  combineShader->pixelConstBuffer->data.space     = 1;
  combineShader->pixelConstBuffer->data.threshold = 0.8f;
  Update();
}
void dxBloomEffect::Update()
{
  thresholdShader->Bound();
  thresholdShader->pixelConstBuffer->Update();
  gaussianShader->Bound();
  gaussianShader->pixelConstBuffer->Update();
  combineShader->Bound();
  combineShader->pixelConstBuffer->Update();
}
void dxBloomEffect::Render()
{
  thresholdShader->pixelConstBuffer->data.dx        = 1.f / dxRenderer::instance()->m_option.width;
  thresholdShader->pixelConstBuffer->data.dy        = 1.f / dxRenderer::instance()->m_option.height;
  thresholdShader->pixelConstBuffer->data.space     = 1;
  thresholdShader->pixelConstBuffer->data.threshold = 0.0f;
  thresholdShader->Bound();
  thresholdShader->pixelConstBuffer->Update();
  origin->Render();
  thresholdShader->pixelConstBuffer->data.dx        = 1.f / dxRenderer::instance()->m_option.width;
  thresholdShader->pixelConstBuffer->data.dy        = 1.f / dxRenderer::instance()->m_option.height;
  thresholdShader->pixelConstBuffer->data.space     = 1;
  thresholdShader->pixelConstBuffer->data.threshold = 0.8f;
  thresholdShader->Bound();
  thresholdShader->pixelConstBuffer->Update();
  t_filter->Render();
  gaussianShader->Bound();
  gaussianShader->pixelConstBuffer->Update();
  g_filter1->Render();
  g_filter2->Render();
  g_filter3->Render();
  g_filter4->Render();
  g_filter5->Render();
  g_filter6->Render();
  c_filter->Render();
}
}  // namespace libEngine