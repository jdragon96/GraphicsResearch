#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__

// #define SKINNED

// Skinned mesh에 대한 변환행렬
#ifdef SKINNED
StructuredBuffer<float4x4> boneTransforms : register(t9);
#endif

/********************* input output *********************/
struct VSInput
{
    float3 vertexPos : POSITION0; //모델 좌표계의 위치 position
    float4 vertexColor: COLOR0;
    float2 vertexTexCoord : TEXTURECOORD0;
    float3 vertexNormal : NORMAL0; // 모델 좌표계의 normal
    
// #ifdef SKINNED
//     float4 boneWeights0 : BLENDWEIGHT0;
//     float4 boneWeights1 : BLENDWEIGHT1;
//     uint4 boneIndices0 : BLENDINDICES0;
//     uint4 boneIndices1 : BLENDINDICES1;
// #endif
};

struct PSInput
{
    float4 posProj : SV_POSITION; // Screen position
    float3 posWorld : POSITION0; // World position (조명 계산에 사용)
    float4 color : COLOR0; // Screen position
    float3 normalWorld : NORMAL0;
    float2 texcoord : TEXCOORD0;
    float3 tangentWorld : TANGENT0;
    float3 posModel : POSITION1; // Volume casting 시작점
};

/********************* Const buffer *********************/
cbuffer GlobalConstants : register(b0)
{
    matrix view;
    matrix proj;
    matrix invWorld;
    // matrix invViewProj; // Proj -> World
    // matrix invView;

    // float3 eyeWorld;
    // float strengthIBL;

    // int textureToDraw = 0; // 0: Env, 1: Specular, 2: Irradiance, 그외: 검은색
    // float envLodBias = 0.0f; // 환경맵 LodBias
    // float lodBias = 2.0f; // 다른 물체들 LodBias
    // float globalTime;
    
    // Light lights[MAX_LIGHTS];
};


cbuffer VertexConstantData: register(b1)
{
    matrix world;
    matrix worldInv;
    float scale;
    float3 dumyy1;
}

cbuffer GlobalPixelConst : register(b2)
{
  float3     eyeWorld;
  float      time;
}


/////////////////////////////////////////////// Blinn phong shader
struct Light
{
    float3 direction;
    float dummy1;
    float3 position;
    float dummy2;
};

struct Material
{
    float3 ambient;
    float ambientFactor;
    float3 diffuse;
    float shininess;
    float3 specular;
    float specularFactor;
};

cbuffer PixelConstBuffer : register(b5)
{
  Light    light;
  Material mat;
  int      useBP;
  int      useAmbient;
  int      useDiffuse;
  int      useTexture;
}

cbuffer FilterConstBuffer: register(b6)
{
  float dx;
  float dy;
  float threshold;
  float strength;
}

#endif