#include "Common.hlsli"

Texture2D g_baseTexture : register(t0);
SamplerState g_sampler : register(s0);

static const float weights[5] = {0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f};

float4 main(PSInput input) : SV_TARGET
{
  float3 color = float3(0,0,0);
  int i;
  for(int i=0; i<5; ++i)
  {
    color += weights[i] * g_baseTexture.Sample(g_sampler, input.texcoord + float2(dx,0) * float(i-2));
  }
  return float4(color, 1);
}