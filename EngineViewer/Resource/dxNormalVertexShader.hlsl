#include "dxCommon.hlsli"

cbuffer NormalVertexConstantBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    matrix invTranspose;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.pos, 1.0f);

    // Normal 먼저 변환
    float4 normal = float4(input.normal, 0.0f);
    output.normal = normalize(mul(normal, invTranspose).xyz);
    
    pos = mul(pos, model);
    float t = input.texturecoord.x;
    
    pos.xyz += (output.normal * t * 1);

    output.posWorld = pos.xyz;
    
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.posProj = pos;
    output.textureCoord = input.texturecoord;
    
    output.color = float4(float3(1.0, 1.0, 0.0) * (1.0 - t) + float3(1.0, 0.0, 0.0) * t, 1.f);

    return output;
}
