#ifndef __MATH_HLSLI__
#define __MATH_HLSLI__

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

float3 NormalFromTexture(float3 normTexture, float3 normalWorld, float3 posWorld)
{
  normTexture = 2.0 * normTexture - 1.0; // 범위 조절 [-1.0, 1.0]
  float3x3 TBN = NormalTransfomrMatrixWithTranslate(normalWorld, posWorld);
  return normalize(mul(normTexture, TBN));
  return normalWorld;
}

#endif