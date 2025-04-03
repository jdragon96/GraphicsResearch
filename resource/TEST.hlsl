cbuffer MouseCursorConstBuffer : register(b5)
{
  float2 cursorPos;
  int    isMousePressed;
  int    dummy1;
};

struct VSInput
{
    float3 vertexPos : POSITION0;
    float4 vertexColor: COLOR0;
    float2 vertexTexCoord : TEXTURECOORD0;
    float3 vertexNormal : NORMAL0;
};

struct PSInput
{
    float4 pos: SV_POSITION;
};

PSInput main(VSInput input)
{
  PSInput output;
  output.pos.x = cursorPos.x;
  output.pos.y = cursorPos.y;
  output.pos.z = 1.f;
  output.pos.w = 1.f;
  return output;
}