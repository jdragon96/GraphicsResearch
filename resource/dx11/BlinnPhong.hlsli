#include "Common.hlsli"

cbuffer PixelConstBuffer : register(b5)
{
  Light    light;
  Material mat;
  int      useBP;
  int      useAmbient;
  int      useDiffuse;
  int      useTexture;
}

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