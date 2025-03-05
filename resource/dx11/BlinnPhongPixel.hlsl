#include "Common.hlsli"

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
    ambientColor = mat.ambient * mat.ambientFactor;
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

  return float4(ambientColor + diffuseColor + specularColor, 1);
}
