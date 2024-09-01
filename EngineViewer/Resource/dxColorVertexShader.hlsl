struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float2 texturecoord : TEXTURECOORD0;
    float3 normal : NORMAL0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL0;
    float2 textureCoord : TEXTURECOORD0;
};

// cbuffer: Constant buffer
// register(b0): GPI 레지스터 b0에 저장해라

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
    output.posWorld = pos.xyz;
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.posProj = pos;
    output.color = input.color;
    output.textureCoord = input.texturecoord;
    
    float4 normalWorld = float4(input.normal, 0.f);
    output.normal = mul(normalWorld, invTranspose).xyz;
    output.normal = normalize(output.normal);

    return output;
}
