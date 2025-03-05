#include "Common.hlsli"

Texture2D g_baseTexture1 : register(t0);
Texture2D g_baseTexture2 : register(t1);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
  float3 color1 = g_baseTexture1.Sample(g_sampler, input.texcoord).rgb;
  float3 color2 = g_baseTexture2.Sample(g_sampler, input.texcoord).rgb;
  return float4(color1 + color2 * strength,  1.f);
}