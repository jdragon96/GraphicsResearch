#include "Common.hlsli"

Texture2D albedoTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
  return albedoTexture.SampleLevel(g_sampler, input.texcoord, mipmapLevel);
}