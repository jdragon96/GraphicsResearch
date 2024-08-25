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

struct Light
{
    float3 direction;
    float dummy1;
    float3 position;
    float dummy2;
};

struct Material
{
    float3 ambient; // 물체의 고유 색상
    float ambientFactor; // 물체 고유색상 강도
    float3 diffuse; // 분산광 강도(물체가 반사하는 빛 색상)
    float shininess; // 더미
    float3 specular; // 반사광 색상(눈에 반사되는 빛 색상)
    float specularFactor; // 반사광 강도
};