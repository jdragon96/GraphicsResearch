RWTexture2D<float4> gOutput : register(u0);

cbuffer MyBuffer : register(b3)
{
  float3 dummy1;
  float scale;
}

[numthreads(32, 32, 1)]
void main(int3 gID : SV_GroupID, uint3 tID : SV_DispatchThreadID)
{
  // float color = (gID.x / 32.f + gID.y / 32.f) * 0.5f * scale;
  float color = (tID.x / 32.f + tID.y / 32.f) * 0.5f * scale;
  gOutput[tID.xy + gID.xy] = float4(color,color,color,1);
}