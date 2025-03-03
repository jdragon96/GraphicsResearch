#include "Common.hlsli"

PSInput main(VSInput input)
{
  PSInput ps;

#ifdef SKINNED

    // 참고 자료: Luna DX 12 교재
    float weights[8];
    weights[0] = input.boneWeights0.x;
    weights[1] = input.boneWeights0.y;
    weights[2] = input.boneWeights0.z;
    weights[3] = input.boneWeights0.w;
    weights[4] = input.boneWeights1.x;
    weights[5] = input.boneWeights1.y;
    weights[6] = input.boneWeights1.z;
    weights[7] = input.boneWeights1.w;

    uint indices[8]; // 힌트: 꼭 사용!
    indices[0] = input.boneIndices0.x;
    indices[1] = input.boneIndices0.y;
    indices[2] = input.boneIndices0.z;
    indices[3] = input.boneIndices0.w;
    indices[4] = input.boneIndices1.x;
    indices[5] = input.boneIndices1.y;
    indices[6] = input.boneIndices1.z;
    indices[7] = input.boneIndices1.w;

    float3 vertexPos = float3(0.0f, 0.0f, 0.0f);
    float3 vertexNormal = float3(0.0f, 0.0f, 0.0f);
    float3 vertexTangent = float3(0.0f, 0.0f, 0.0f);

    // Uniform Scaling 가정
    // (float3x3)boneTransforms 캐스팅으로 Translation 제외
    for (int i = 0; i < 8; ++i) {
        vertexPos += weights[i] * mul(float4(input.vertexPos, 1.0f), boneTransforms[indices[i]]).xyz;
        // vertexNormal += weights[i] * mul(input.vertexNormal, (float3x3)boneTransforms[indices[i]]);
        // vertexTangent += weights[i] * mul(input.vertexTexCoord, (float3x3)boneTransforms[indices[i]]);
    }
    input.vertexPos = vertexPos;
    // input.vertexNormal = vertexNormal;
#endif

    float4 pos = float4(input.vertexPos, 1.0f);
    // pos *= scale;
    pos.x *= scale;
    pos.y *= scale;
    pos.z *= scale;
    pos = mul(pos, world);
    ps.posWorld = pos.xyz;
    pos = mul(pos, view);
    pos = mul(pos, proj);
    ps.posProj = pos;
    ps.color = input.vertexColor;
    ps.texcoord = input.vertexTexCoord;
    
    float4 normalWorld = float4(input.vertexNormal, 0.f);
    ps.normalWorld = mul(normalWorld, worldInv).xyz;
    ps.normalWorld = normalize(ps.normalWorld);

  return ps;
}