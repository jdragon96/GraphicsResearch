#include "Common.hlsli"

TextureCube g_albedoTexture : register(t0);
TextureCube g_specularTexture : register(t1);
TextureCube g_diffuseTexture : register(t2);
SamplerState g_sampler : register(s0);

cbuffer PixelConstBuffer : register(b5)
{
  int      useAlbedo;
  int      useSpecular;
  int      useDiffuse;
  int      dummy;
}

float4 main(PSInput input) : SV_TARGET
{
  if(useAlbedo)
    return g_albedoTexture.Sample(g_sampler, input.posWorld.xyz);
  else if(useSpecular)
    return g_specularTexture.Sample(g_sampler, input.posWorld.xyz);
  else if(useDiffuse)
    return g_diffuseTexture.Sample(g_sampler, input.posWorld.xyz);
  else
    return float4(0,0,0,1);
}
