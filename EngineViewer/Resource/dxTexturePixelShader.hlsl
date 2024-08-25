 #include "dxCommon.hlsli" // 쉐이더에서도 include 사용 가능

Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

cbuffer PixelConstBuffer : register(b0)
{
    float3 eyeWorld;
    bool useTexture;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    // float3 image = g_texture0.Sample(g_sampler, input.textureCoord).rgb;
	// return float4(image, 1.0);
	// float3 color = g_texture0.Sample(g_sampler, input.textureCoord);
    return g_texture0.Sample(g_sampler, input.textureCoord);
}