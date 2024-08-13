// #include "Common.hlsli" // 쉐이더에서도 include 사용 가능

SamplerState g_sampler : register(s0);

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float4 color : COLOR;
};

cbuffer PixelConstBuffer : register(b0)
{
    float3 eyeWorld;
    bool useTexture;
}

float4 main(PixelShaderInput input) : SV_TARGET
{
    return input.color;
}