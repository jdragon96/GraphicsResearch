#include "Common.hlsli"

PSInput main(VSInput input)
{
    PSInput output;
    float4 pos = float4(input.vertexPos, 1.0f);
    pos = mul(pos, world);
    if(useReflection)
    {
        pos = mul(pos, reflection);
    }
    output.posWorld = pos.xyz;
    pos = mul(pos, view);
    pos = mul(pos, proj);
    output.posProj = pos;
    output.color = input.vertexColor;
    output.texcoord = input.vertexTexCoord;
    
    float4 normalWorld = float4(input.vertexNormal, 0.f);
    output.normalWorld = mul(normalWorld, worldInv).xyz;
    output.normalWorld = normalize(output.normalWorld);

    return output;
}
