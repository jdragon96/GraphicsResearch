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
    float3 ambient; // ��ü�� ���� ����
    float ambientFactor; // ��ü �������� ����
    float3 diffuse; // �л걤 ����(��ü�� �ݻ��ϴ� �� ����)
    float shininess; // ����
    float3 specular; // �ݻ籤 ����(���� �ݻ�Ǵ� �� ����)
    float specularFactor; // �ݻ籤 ����
};