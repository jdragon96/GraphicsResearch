#include "Common.hlsli"

struct VS2GS
{
    float4 posWorld : SV_POSITION;
};

VS2GS main(VSInput input)
{
    VS2GS output;
    output.posWorld = float4(input.vertexPos, 1.0f);
    return output;
}
