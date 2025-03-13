#include "Common.hlsli"

Texture2D heightTexture : register(t0);
SamplerState g_sampler : register(s0);

PSInput main(VSInput input)
{
    PSInput output;
    float4 pos = float4(input.vertexPos, 1.0f);

    // normal vector 업데이트
    float4 normalWorld = float4(input.vertexNormal, 0.f);
    output.normalWorld = mul(normalWorld, worldInv).xyz;
    output.normalWorld = normalize(output.normalWorld);

    // height texture 적용
    float height = heightTexture.SampleLevel(g_sampler, input.vertexTexCoord, 0).r;
    height = height * 2.f - 1.f;
    pos += normalWorld * height * heightScale;

    // 기타
    pos = mul(pos, world);
    output.posWorld = pos.xyz;
    pos = mul(pos, view);
    pos = mul(pos, proj);
    output.posProj = pos;
    output.color = input.vertexColor;
    output.texcoord = input.vertexTexCoord;

    return output;
}
