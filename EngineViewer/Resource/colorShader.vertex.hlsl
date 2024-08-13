// #include "Common.hlsli" // 쉐이더에서도 include 사용 가능

// cbuffer: Constant buffer
// register(b0): GPI 레지스터 b0에 저장해라
struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float2 texturecoord : TEXTURECOORD0;
    float3 normal : NORMAL;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float4 color : COLOR;
};

cbuffer VertexConstBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    matrix invTranspose;
};

// Intrinsic function: 내장 함수
PixelShaderInput main(VertexShaderInput input)
{

    PixelShaderInput output;
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, model);
	//output.posWorld = pos;
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.posProj = pos;
    output.color = input.color;
    //output.texturecoord = input.texturecoord;
    
    float4 normal = float4(input.normal, 0.f);
	//output.normalWorld = mul(normal, invTranspose).xyz;
	//output.normalWorld = normalize(output.normalWorld);

    return output;
}
