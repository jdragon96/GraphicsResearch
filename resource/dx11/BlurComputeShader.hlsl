/****************************
- SV_DispatchThreadID : Thread의 고유 ID
- SV_GroupID : Thread Group의 고유 ID




*****************************/

Texture2D<float4> inputTex : register(t0);
RWTexture2D<float4> gOutput : register(u0);
SamplerState pointClampSS : register(s0);

cbuffer MyBuffer : register(b3)
{
  float3 dummy1;
  int useHorizontal;
}

static float weight[5] = {0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216};

[numthreads(32, 32, 1)]
void main(int3 gID : SV_GroupID, uint3 tID : SV_DispatchThreadID)
{
  float tex_offset = 1.0 / 512.0;
  float2 uv = tID.xy / 512.f;
  float3 result = inputTex.SampleLevel(pointClampSS, uv, 0).rgb * weight[0]; // current fragment's contribution
  if(useHorizontal)
  {
    for(int i = 1; i < 5; ++i)
    {
      result += inputTex.SampleLevel(pointClampSS, uv + float2(tex_offset * i, 0), 0).rgb * weight[i];
      result += inputTex.SampleLevel(pointClampSS, uv - float2(tex_offset * i, 0), 0).rgb * weight[i];
    }
  }
  else
  {
    for(int i = 1; i < 5; ++i)
    {
      result += inputTex.SampleLevel(pointClampSS, uv + float2(0, tex_offset * i), 0).rgb * weight[i];
      result += inputTex.SampleLevel(pointClampSS, uv - float2(0, tex_offset * i), 0).rgb * weight[i];
    }
  }
  gOutput[tID.xy] = float4(result, 1);
}