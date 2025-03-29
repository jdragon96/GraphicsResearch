RWTexture2D<float4> gOutput : register(u0);

cbuffer MyBuffer : register(b3)
{
  float3 dummy1;
  float scale;
}

[numthreads(32, 32, 1)]
void main(int3 gID : SV_GroupID, uint3 tID : SV_DispatchThreadID)
{
  int2 uniqueID = tID.xy;
  int2 center = float2(256, 256);
  float dist = distance(uniqueID, center);
  dist = dist < 128 ? 1 : 0;
  gOutput[uniqueID] = float4(dist,0,0,1);
}