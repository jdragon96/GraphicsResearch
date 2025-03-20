#include "Common.hlsli"
#include "BlinnPhong.hlsli"
#include "PBR.hlsli"
#include "Math.hlsli"

static const float3 Fdielectric = 0.04;

cbuffer PBRBuffer : register(b5)
{
  int      useAlbedoMap;
  int      useAOMap;
  int      useMetallicMap;
  int      useRoughnessMap;
}

cbuffer PixelConstBuffer : register(b6)
{
  Light    light;
  Material mat;
  int      useBP;
  int      useAmbient;
  int      useDiffuse;
  int      useTexture;
}


float4 main(PSInput input) : SV_TARGET
{
  float3 toEye   = normalize(eyeWorld - input.posWorld);
  float3 toLight = normalize(light.position - input.posWorld);

  float3 normTexture = normalTex.Sample(linearSampler, input.texcoord).rgb;
  float3 worldNormal = NormalFromTexture(normTexture, input.normalWorld, input.posWorld);

  float3 albedo = useAlbedoMap ? albedoTex.Sample(linearSampler, input.texcoord).rgb 
                                : mat.ambient;
  float ao = useAOMap ? aoTex.SampleLevel(linearSampler, input.texcoord, 0.0).r : 1.0;
  float metallic = useMetallicMap ? metallicTex.Sample(linearSampler, input.texcoord).r 
                                  : 1.0;
  float roughness = useRoughnessMap ? roughnessTex.Sample(linearSampler, input.texcoord).r 
                                    : 1.0;
  float3 ambientLighting = AmbientLightingByIBL(albedo, worldNormal, toEye, ao,
                                                metallic, roughness, Fdielectric);

  float3 directLighting = float3(0,0,0);
  float3 halfWay = normalize(toEye + toLight);
  float NdotL = max(0.0, dot(worldNormal, toLight));
  float NdotH = max(0.0, dot(worldNormal, halfWay));
  float NdotE = max(0.0, dot(worldNormal, toEye));

  float3 F0 = lerp(Fdielectric, albedo, metallic);
  float3 F = SchlickFresnel(F0, NdotH);
  float3 kd = lerp(float3(1,1,1) - F, float3(0,0,0), metallic); // 산란광 계산
  float3 diffuseBRDF = kd * albedo;
  float D = NdfGGX(NdotH, roughness);
  float3 G = SchlickGGX(NdotL, NdotE, roughness);
  float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotL * NdotE); 
  // float3 radiance = light[i].radiance * saturate((light[i].fallOffEnd - length(lightVec)) / (light[i].fallOffEnd - light[i].fallOffStart));
  // directLighting += ((diffuseBRDF + specularBRDF) * 1.0 * NdotL);
  directLighting += ((diffuseBRDF + specularBRDF));

  return float4(ambientLighting + directLighting, 1);
  // return float4(color,0) + albedoTexture.SampleLevel(g_sampler, input.texcoord, mipmapLevel); 
}