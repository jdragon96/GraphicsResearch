#include "Common.hlsli"

Texture2D g_baseTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
  return g_baseTexture.Sample(g_sampler, input.texcoord);
}