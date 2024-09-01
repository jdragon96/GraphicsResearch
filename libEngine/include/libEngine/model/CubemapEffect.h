#pragma once

#include <string>
#include "libEngine/utils/macro.h"
#include "libEngine/model/ConstBase.h"

namespace libEngine
{
namespace CubemapEffect
{
struct VertexConstBuf : public ConstBase
{
  Mat4 model;
  Mat4 view;
  Mat4 projection;

  virtual void UpdateModel(Mat4 m) override
  {
    switch (MemDB::CurrentEngineType)
    {
      case EngineType::OPENGL:
        model = m;
        break;
      case EngineType::DX11:
        model = m.transpose();
        break;
    }
  }

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

struct PixelConstBuf : public ConstBase
{
  Vec4 dummy;
};

const std::string glVertexShader = R"(
#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normalVector;

out VS_OUT {
  vec3 posWorld;
  vec2 textureCoord;
} vs_out;

layout (std140, binding=0) uniform VertexConstBuffer
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

void main() {
  gl_Position = projection * view * model * vec4(vertexPosition, 1);
  vs_out.posWorld = (model * vec4(vertexPosition, 1)).xyz;
  vs_out.textureCoord = texCoord;
}
)";

const std::string glPixelShader = R"(
#version 460 core

uniform samplerCube skybox;

in VS_OUT {
  vec3 posWorld;
  vec2 textureCoord;
} vs_out;
out vec4 FragColor;

void main(){
    FragColor = texture(skybox, vs_out.posWorld);
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

struct PixelShaderInput
{
    float4 posProj: SV_POSITION;
    float4 posWorld : POSITION0;
};

cbuffer VertexConstBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    output.posWorld = mul(float4(input.pos, 1), model);
    output.posProj = mul(mul(output.posWorld, view), projection);
    return output;
}
    )";

const std::string dxPixelShader = R"(
TextureCube g_textureCube0 : register(t0);
SamplerState g_sampler : register(s0);

struct PixelShaderInput
{
    float4 posProj: SV_POSITION;
    float4 posWorld : POSITION0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    return g_textureCube0.Sample(g_sampler, input.posWorld.xyz);
}
    )";
}  // namespace CubemapEffect
}  // namespace libEngine