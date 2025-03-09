#include "Common.hlsli"

struct GS2PS
{
  float4 posProj : SV_POSITION; // Screen position
  float3 posWorld : POSITION0; // World position (조명 계산에 사용)
  uint primID : SV_PrimitiveID;
};

float4 main(GS2PS input) : SV_TARGET
{
  return float4(1,0,0,1);
}