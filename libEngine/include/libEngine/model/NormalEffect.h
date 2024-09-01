#pragma once

#include <string>
#include "libEngine/MemDB.h"

namespace libEngine
{
namespace NormalEffect
{
struct VertexConstBuf : public ConstBase
{
  Vec4 dummy;
};

struct PixelConstBuf : public ConstBase
{
  Vec4 dummy;
};
struct GeometryConstBuf : public ConstBase
{
  Mat4  model;
  Mat4  view;
  Mat4  projection;
  Mat4  invTranspose;
  Vec3  startColor;
  float normalLength;
  Vec3  endColor;
  float dummy1;

  virtual void UpdateCamera(CameraBuffer::SharedPtr cam) override
  {
    switch (MemDB::CurrentEngineType)
    {
      case EngineType::OPENGL:
        projection = *cam->GetProjMatPtr();
        view       = *cam->GetViewMatPtr();
        break;
      case EngineType::DX11:
        projection = cam->GetProjMatPtr()->transpose();
        view       = cam->GetViewMatPtr()->transpose();
        break;
    }
  }
};

const std::string glVertexShader = R"(
#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normalVector;

out VS_OUT {
  vec3 normal;
} vs_out;

void main() {
  vs_out.normal = normalVector;
  gl_Position = vec4(vertexPosition, 1);
}
)";

const std::string glGeometryShader = R"(
#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

layout (std140, binding=2) uniform GeometryConstBuffer
{
  mat4 model;
  mat4 view;
  mat4 projection;
  mat4 invModel;
  vec3 startColor;
  float normalLength;
  vec3 endColor;
  float dummy1;
};

in VS_OUT {
  vec3 normal;
} gs_in[];

out vec3 normalColor;

void main() {
  // 1. start position
  vec4 posWorld = model * gl_in[0].gl_Position;
  gl_Position = projection * view * posWorld;
  normalColor = startColor;
  EmitVertex();
  
  // 2. end position
  vec4 transformedNormal = invModel * vec4(gs_in[0].normal, 0);
  gl_Position = projection * view * (posWorld + transformedNormal * normalLength);
  normalColor = endColor;
  EmitVertex();
  EndPrimitive();
}
)";

const std::string glPixelShader = R"(
#version 460 core

in vec3 normalColor;
out vec4 FragColor;

void main(){
    FragColor = vec4(normalColor, 1);
}
)";

const std::string dxVertexShaader = R"(
struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float2 texturecoord : TEXTURECOORD0;
    float3 normal : NORMAL0;
};

struct GeometryShaderInput
{
    float4 posModel : POSITION;
    float3 normal : NORMAL0;
};

GeometryShaderInput main(VertexShaderInput input)
{
    GeometryShaderInput output;
    output.posModel = float4(input.pos, 1.0);
    output.normal = input.normal;
    return output;
}
    )";

const std::string dxGeometryShader = R"(
struct GeometryShaderInput
{
    float4 posModel : POSITION;
    float3 normal : NORMAL0;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    uint primID : SV_PrimitiveID;
};

cbuffer GeometryConstBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    matrix invTranspose;
    float3 startColor;
    float normalLength;
    float3 endColor;
    float dummy1;
};

[maxvertexcount(2)]
void main(
  point GeometryShaderInput input[1], 
  uint primID : SV_PrimitiveID,
  inout LineStream<PixelShaderInput> outputStream)
{
    PixelShaderInput output;
    output.color = float4(startColor, 1.0);
    output.primID = primID;
    
    // 1. 노멀 원점
    float4 posWorld = mul(input[0].posModel, model);
    output.pos = mul(mul(posWorld, view), projection);
    outputStream.Append(output);
    
    // 2. 노멀 방향벡터
    output.color = float4(endColor, 1.0);
    float4 norm = normalize(mul(float4(input[0].normal, 0.0), invTranspose));
    output.pos = mul(mul(posWorld + norm * normalLength, view), projection);
    outputStream.Append(output);
    outputStream.RestartStrip();
}
    )";

const std::string dxPixelShader = R"(
struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    uint primID : SV_PrimitiveID;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return input.color;
}
    )";
}  // namespace NormalEffect
}  // namespace libEngine