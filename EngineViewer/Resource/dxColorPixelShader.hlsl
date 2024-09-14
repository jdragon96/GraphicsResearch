Texture2D g_texture0 : register(t0);
TextureCube g_textureDiffuse : register(t1);
TextureCube g_textureSpecular : register(t2);

SamplerState g_sampler : register(s0);

#define INV_PI 0.3183129;


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

float3 FresnelEffect(float3 fresnel, float3 light, float3 normal, float3 toEye)
{
    float NdotE = saturate(dot(normal, toEye));
    // 1. ������ �������� ��������
    // 2. ���� �������� �� ����
    //return fresnel;
    //return (fresnel + (1.0f - fresnel) * pow((1.0f - NdotE), 5.0f));
    return fresnel * 0.3 + (1.0f - fresnel) * pow((1.0f - NdotE), 5.0f);
}

float TrowbridgeReitzGGX(float NdotH, float roughness)
{
    float a = NdotH * roughness;
    float k = roughness / (1.0f - NdotH * NdotH + a * a);
    return k * k * 1.0f * INV_PI;

}

cbuffer PixelConstBuffer : register(b0)
{
    float3 camWorld;
    bool useTexture;
    Light light;
    Material material;
}

float4 main(PixelShaderInput input) : SV_TARGET
{
    // 1. Ambient color
    float3 ambientColor = material.ambient * material.ambientFactor;
    // 2. Diffuse color(�л걤, ���ݻ�, ������ ������ ���� �ݻ�)
    float3 toLight = normalize(light.position - input.posWorld);
    float lambertFactor = max(dot(input.normal, toLight), 0);
    float3 diffuseColor = lambertFactor * material.diffuse * material.shininess;
    // 3. specular color(�ݻ籤, ���� ����� ��ġ�� ���� ���ϰ� �ݻ�)
    //float3 lightStrength = max(dot(normalize(-light.direction), input.normal), 0) * light.strength;
    float3 toCamera = normalize(camWorld - input.posWorld);
    float halfVector = normalize(toCamera + normalize(-light.direction));
    //float halfVector = normalize(toCamera + toLight);
    float NdotH = dot(toCamera, input.normal);
    float3 specularColor = pow(material.specularFactor, max(NdotH, 0)) * material.specular;
    
    float3 blinnphong = ambientColor + diffuseColor + specularColor;
    
    
    // return g_texture0.Sample(g_sampler, input.textureCoord);
    float4 diffuse_texture = g_textureDiffuse.Sample(g_sampler, input.normal);
    float4 specular_texture = g_textureSpecular.Sample(g_sampler, reflect(-toCamera, input.normal)) * 0.33f;
    specular_texture.xyz += FresnelEffect(float3(1.f, 0.f, 0.f), float3(1, 1, 1), input.normal, toCamera);
    return specular_texture;
    return g_textureDiffuse.Sample(g_sampler, input.normal);
    if (useTexture)
    {
        //return g_texture0.Sample(g_sampler, input.textureCoord) * float4(newColor, 1);
        return float4(blinnphong, 1) + g_textureDiffuse.Sample(g_sampler, input.normal);
    }
    else
    {
        return float4(blinnphong, 1) + g_textureDiffuse.Sample(g_sampler, input.normal);

    }
}
