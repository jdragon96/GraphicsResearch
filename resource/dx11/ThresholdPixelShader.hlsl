#include "Common.hlsli"

Texture2D g_baseTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
  float3 color = g_baseTexture.Sample(g_sampler, input.texcoord).rgb;
  float l = (color.r + color.g + color.b) / 3.f;
  return l > threshold ? float4(color, 1.f) : float4(0.f,0.f,0.f,1.f);
}