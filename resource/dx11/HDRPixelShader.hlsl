#include "Common.hlsli"
#include "BlinnPhong.hlsli"

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D aoTexture : register(t2);
SamplerState g_sampler : register(s0);

float3x3 NormalTransfomrMatrixWithTranslate(float3 axisZ, float3 posWorld) {
  float3 axisX = normalize(float3(0, axisZ.y, axisZ.z));
  float l = length(axisX);
  axisX.x = posWorld.x;
  if (l < 0.00001) {
    axisX.y = 1 + posWorld.y;
    axisX.z = 0 + posWorld.z;
  } else {
    axisX.y = -axisX.z / l + posWorld.y;
    axisX.z = axisX.y / l + posWorld.z;
  }
  float3 axisY = normalize(cross(axisZ, axisX));
  axisY.x += posWorld.x;
  axisY.y += posWorld.y;
  axisY.z += posWorld.z;
  axisZ.x += posWorld.x;
  axisZ.y += posWorld.y;
  axisZ.z += posWorld.z;
  return float3x3(axisX, axisY, axisZ);
}

float3 GetNormal(float3 normalWorld, float2 texcoord, float3 posWorld)
{
    // if(useNormalMap)
    // {

    // }
    float3 normal = normalTexture.SampleLevel(g_sampler, texcoord, 0.0).rgb;
    normal = 2.0 * normal - 1.0; // 범위 조절 [-1.0, 1.0]
    float3x3 TBN = NormalTransfomrMatrixWithTranslate(normalWorld, posWorld);
    return normalize(mul(normal, TBN));
    return normalWorld;
}

float4 main(PSInput input) : SV_TARGET
{
  float3 toEye   = normalize(eyeWorld - input.posWorld);
  float3 toLight = normalize(light.position - input.posWorld);

  float3 ambientColor  = float3(0, 0, 0);
  float3 diffuseColor  = float3(0, 0, 0);
  float3 specularColor = float3(0, 0, 0);
  float3 worldNormal = GetNormal(input.normalWorld, input.texcoord, input.posWorld);

  // 1. ambient color
  if (bool(useAmbient))
  {
    ambientColor = mat.ambient * mat.ambientFactor;
  }
  // 2. diffuse color
  if (bool(useDiffuse))
  {
    diffuseColor = mat.diffuse * diffuseFactor(toLight, worldNormal);
  }
  // 3. specular color
  if (bool(useBP))
  {
    specularColor = mat.specular * mat.specularFactor * BPReflection(toLight, toEye, worldNormal);
  }

  float3 color = ambientColor + diffuseColor + specularColor;
  return float4(color,0) + albedoTexture.SampleLevel(g_sampler, input.texcoord, mipmapLevel); 
}