Texture2D g_texture0 : register(t0);
TextureCube g_textureCube : register(t1);

SamplerState g_sampler : register(s0);


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
    // 2. Diffuse color(분산광, 난반사, 균일한 세기의 빛이 반사)
    float3 toLight = normalize(light.position - input.posWorld);
    float lambertFactor = max(dot(input.normal, toLight), 0);
    float3 diffuseColor = lambertFactor * material.diffuse * material.shininess;
    // 3. specular color(반사광, 눈과 가까운 위치는 빛이 강하게 반사)
    //float3 lightStrength = max(dot(normalize(-light.direction), input.normal), 0) * light.strength;
    float3 toCamera = normalize(camWorld - input.posWorld);
    float halfVector = normalize(toCamera + normalize(-light.direction));
    //float halfVector = normalize(toCamera + toLight);
    float NdotH = dot(toCamera, input.normal);
    float3 specularColor = pow(material.specularFactor, max(NdotH, 0)) * material.specular;
    
    float3 blinnphong = ambientColor + diffuseColor + specularColor;
    
    
    
    // return g_texture0.Sample(g_sampler, input.textureCoord);
    return g_textureCube.Sample(g_sampler, reflect(-toCamera, input.normal));
    return g_textureCube.Sample(g_sampler, input.normal);
    if (useTexture)
    {
        //return g_texture0.Sample(g_sampler, input.textureCoord) * float4(newColor, 1);
        return float4(blinnphong, 1) + g_textureCube.Sample(g_sampler, input.normal);
    }
    else
    {
        return float4(blinnphong, 1) + g_textureCube.Sample(g_sampler, input.normal);

    }
}
