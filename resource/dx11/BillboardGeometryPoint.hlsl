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
};

[maxvertexcount(100)] // 최대 출력 Vertex 갯수
void main(point VS2GS input[1], uint primID : SV_PrimitiveID, inout PointStream<GS2PS> outputStream)
{
    GS2PS output;
    
    for (int i = 0; i < 100; i ++)
    {
        output.posWorld = (input[0].posWorld + float4(0.0, 0.003, 0.0, 0.0) * float(i)).rgb;
        output.posProj = mul(float4(output.posWorld, 1.0), viewGeom);
        output.posProj = mul(output.posProj, projGeom);
        output.primID = primID;
        outputStream.Append(output);
    }
    // 주의: GS는 Triangle Strips으로 출력합니다.
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/triangle-strips
    // outputStream.RestartStrip(); // Strip을 다시 시작
}

