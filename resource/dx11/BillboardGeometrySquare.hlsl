#include "Common.hlsli"

/*
* Stream 가능한 옵션
* PointStream : 포인트
* LineStream : 선
* TriangleStream : 삼각형
*/
struct VS2GS
{
    float4 posWorld : SV_POSITION;
};

struct GS2PS
{
    float4 posProj : SV_POSITION; // Screen position
    float3 posWorld : POSITION0; // World position (조명 계산에 사용)
    float2 textureCoordination: TEXTURECOORDINATION0;
    uint primID : SV_PrimitiveID;
};

cbuffer BillboardPointsConstantData : register(b7)
{
    matrix viewGeom;
    matrix projGeom;
    float3 eyeWorldPos;
    float dummy1;
    float3 eyeUpVector;
    float dummy2;
};

void makeTriangle(
    float4 v1, 
    float4 v2, 
    float4 v3, 
    inout TriangleStream<GS2PS> outputStream, 
    GS2PS output)
{
    output.posProj = v1;
    outputStream.Append(output);
    output.posProj = v2;
    outputStream.Append(output);
    output.posProj = v3;
    outputStream.Append(output);
    outputStream.RestartStrip();
};

[maxvertexcount(6)]
void main(point VS2GS input[1], uint primID : SV_PrimitiveID, 
            inout TriangleStream<GS2PS> outputStream)
{
    GS2PS output;
    output.primID = primID;
    output.posWorld = input[0].posWorld.rgb;
    float3 eyeTo = normalize(output.posWorld - eyeWorldPos);
    float3 rightVector = normalize(cross(eyeTo, eyeUpVector));
    float3 upVector = normalize(eyeUpVector);
    float4 u1 = mul(mul(float4(output.posWorld -rightVector - upVector, 1), viewGeom), projGeom);
    float4 u2 = mul(mul(float4(output.posWorld -rightVector + upVector, 1), viewGeom), projGeom);
    float4 u3 = mul(mul(float4(output.posWorld + rightVector + upVector, 1), viewGeom), projGeom);
    float4 u4 = mul(mul(float4(output.posWorld + rightVector - upVector, 1), viewGeom), projGeom);
    {
        output.textureCoordination = float2(0,1);
        output.posProj = u1;
        outputStream.Append(output);
        output.textureCoordination = float2(1,0);
        output.posProj = u3;
        outputStream.Append(output);
        output.textureCoordination = float2(0,0);
        output.posProj = u2;
        outputStream.Append(output);
        outputStream.RestartStrip();   
    }
    {
        output.textureCoordination = float2(0,1);
        output.posProj = u1;
        outputStream.Append(output);
        output.textureCoordination = float2(1,1);
        output.posProj = u4;
        outputStream.Append(output);
        output.textureCoordination = float2(1,0);
        output.posProj = u3;
        outputStream.Append(output);
        outputStream.RestartStrip();   
    }
    // makeTriangle(u1, u3, u2, outputStream, output);
    // makeTriangle(u1, u4, u3, outputStream, output);
    // 주의: GS는 Triangle Strips으로 출력합니다.
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/triangle-strips
    // outputStream.RestartStrip(); // Strip을 다시 시작
}

