#include "Common.hlsli"

PSInput main(VSInput input)
{
  PSInput output;
  output.posWorld = input.vertexPos;
  output.posProj = float4(input.vertexPos, 1);
  output.texcoord = input.vertexTexCoord;
  return output;
}
