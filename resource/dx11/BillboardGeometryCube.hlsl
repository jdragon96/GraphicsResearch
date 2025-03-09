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
    uint primID : SV_PrimitiveID;
};

cbuffer BillboardPointsConstantData : register(b7)
{
    matrix viewGeom;
    matrix projGeom;
    float3 dummy;
    float length;
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

[maxvertexcount(36)]
void main(point VS2GS input[1], uint primID : SV_PrimitiveID, 
            inout TriangleStream<GS2PS> outputStream)
{
    GS2PS output;
    output.primID = primID;
    output.posWorld = input[0].posWorld.rgb;
    float hl = length*0.5f;
    float4 u1 = mul(mul(input[0].posWorld + float4(-hl,-hl,hl, 1), viewGeom), projGeom);
    float4 u2 = mul(mul(input[0].posWorld + float4(-hl,hl,hl, 1), viewGeom), projGeom);
    float4 u3 = mul(mul(input[0].posWorld + float4(hl,hl,hl, 1), viewGeom), projGeom);
    float4 u4 = mul(mul(input[0].posWorld + float4(hl,-hl,hl, 1), viewGeom), projGeom);
    float4 d1 = mul(mul(input[0].posWorld + float4(-hl,-hl,-hl, 1), viewGeom), projGeom);
    float4 d2 = mul(mul(input[0].posWorld + float4(-hl,hl,-hl, 1), viewGeom), projGeom);
    float4 d3 = mul(mul(input[0].posWorld + float4(hl,hl,-hl, 1), viewGeom), projGeom);
    float4 d4 = mul(mul(input[0].posWorld + float4(hl,-hl,-hl, 1), viewGeom), projGeom);
    makeTriangle(u1, u3, u2, outputStream, output);
    makeTriangle(u1, u4, u3, outputStream, output);
    makeTriangle(d1, u4, u1, outputStream, output);
    makeTriangle(d1, d4, u4, outputStream, output);
    makeTriangle(d2, u1, u2, outputStream, output);
    makeTriangle(d2, d1, u1, outputStream, output);
    makeTriangle(d3, u2, u3, outputStream, output);
    makeTriangle(d3, d2, u2, outputStream, output);
    makeTriangle(d4, u3, u4, outputStream, output);
    makeTriangle(d4, d3, u3, outputStream, output);
    makeTriangle(d4, d2, d3, outputStream, output);
    makeTriangle(d4, d1, d2, outputStream, output);
    // 주의: GS는 Triangle Strips으로 출력합니다.
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/triangle-strips
    // outputStream.RestartStrip(); // Strip을 다시 시작
}

