#include "Common.hlsli"

Texture2D g_baseTexture : register(t0);
TextureCube g_textureCube0 : register(t1);
SamplerState g_sampler : register(s0);

float diffuseFactor(float3 toLight, float3 normal)
{
  return max(dot(toLight, normal), 0.f);
}

float BPReflection(float3 toLight, float3 toEye, float3 normal)
{
  float3 halfWay = normalize(toLight + toEye);
  return pow(max(dot(normal, halfWay), 0.f), 16.f);
}

float PhongReflection(float3 toLight, float3 toEye, float3 normal)
{
  float3 reflectDir = reflect(-toLight, normal);
  return pow(max(dot(toEye, reflectDir), 0.0), 8.0);
}

// Gold: (1.0, 0.71, 0.29)
float3 SchlickFresnel(float3 fresnelR0, float3 normal, float3 toEye)
{
  float normalDotView = saturate(dot(normal, toEye));
  // toEye, normal가 수평일 경우 : 0
  // toEye, normal가 수직일 경우 : 1
  float f0 = 1.0f - normalDotView;
  // f0가 0일 때 : 고유 texture가 잘 보이게
  // f0가 1일 때 : 
  return fresnelR0 + (1.0f - fresnelR0) * pow(f0, 2.0);
}

float4 main(PSInput input) : SV_TARGET
{
  float3 toEye   = normalize(eyeWorld - input.posWorld);
  float3 toLight = normalize(light.position - input.posWorld);

  float3 ambientColor  = float3(0, 0, 0);
  float3 diffuseColor  = float3(0, 0, 0);
  float3 specularColor = float3(0, 0, 0);
  // 1. ambient color
  if (bool(useAmbient))
  {
    if(bool(useTexture))
    {
      ambientColor = g_baseTexture.Sample(g_sampler, input.texcoord).xyz;
      ambientColor = SchlickFresnel(ambientColor, input.normalWorld, toEye);
    }
    else
    {
      ambientColor = mat.ambient * mat.ambientFactor;
    }
  }
  // 2. diffuse color
  if (bool(useDiffuse))
  {
    diffuseColor = mat.diffuse * diffuseFactor(toLight, input.normalWorld);
  }
  // 3. specular color
  if (bool(useBP))
  {
    specularColor = mat.specular * mat.specularFactor * BPReflection(toLight, toEye, input.normalWorld);
  }

  // 4. fresnel effect
  

  // 5. environment mapping
  
  float4 blinnphong = float4(ambientColor + diffuseColor + specularColor, 1);
  float4 envMap = g_textureCube0.Sample(g_sampler, reflect(-toEye, input.normalWorld));

  return envMap + blinnphong;
}