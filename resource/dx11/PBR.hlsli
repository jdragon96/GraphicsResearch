/**
* Cubemap texture 
* - specularIBLTex: 
* 
*/
#ifndef __PBR_HLSLI__
#define __PBR_HLSLI__

#include "Common.hlsli"

float3 SchlickFresnel(float3 F0, float NdotH)
{
  return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
}

float3 SpecularIBL(float3 albedo, float3 normalWorld, float3 pixelToEye, float metallic, float roughness)
{
    float2 specularBRDF = brdfTex.Sample(
      clampSampler, 
      float2(dot(normalWorld, pixelToEye), 1.0 - roughness)).rg;
    
    // 앞에서 사용했던 방법과 동일
    float3 specularIrradiance =
        specularIBLTex
            .SampleLevel(linearSampler,
                         reflect(-pixelToEye, normalWorld), roughness * 10.0f).rgb;
    const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
    float3 F0 = lerp(Fdielectric, albedo, metallic);

    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}

float3 DiffuseIBL(float3 albedo, float3 normalWorld, float3 pixelToEye,
                  float metallic, float3 Fdielectric)
{
  float3 F0 = lerp(Fdielectric, albedo, metallic);
  float3 F = SchlickFresnel(F0, max(0.0, dot(normalWorld, pixelToEye)));
  // metallic이 커지면, diffuse는 감소
  float3 kd = lerp(1.0 - F, 0.0, metallic);
  float3 irradiance = irradianceIBLTex.Sample(linearSampler, normalWorld).rgb;
  return kd * albedo * irradiance;
}

float3 AmbientLightingByIBL(float3 albedo, float3 normalW, float3 pixelToEye, float ao,
                            float metallic, float roughness, float3 Fdielectric)
{
  float3 diffuseIBL = DiffuseIBL(albedo, normalW, pixelToEye, metallic, Fdielectric);
  float3 specularIBL = SpecularIBL(albedo, normalW, pixelToEye, metallic, roughness);
  return (diffuseIBL + specularIBL) * ao;
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float NdfGGX(float NdotH, float roughness)
{
  float alpha = roughness * roughness;
  float alphaSq = alpha * alpha;
  float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;
  return alphaSq / (3.141582 * denom * denom);
}

float SchlickG1(float NdotH, float k) {
  return NdotH / (NdotH * (1.0f - k) + k);
}

float SchlickGGX(float NdotI, float NdotO, float roughness)
{
  float r = roughness + 1.0;
  float k = (r * r) / 8.0;
  return SchlickG1(NdotI, k) * SchlickG1(NdotO, k);
}

#endif