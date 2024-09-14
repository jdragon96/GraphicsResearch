#include "libEngine/opengl/postprocessing/glBloomEffect.h"

namespace libEngine
{
glBloomEffect::glBloomEffect()
{
}
glBloomEffect ::~glBloomEffect()
{
}
void glBloomEffect::Initialize()
{
  const std::string vertexShader = R"(
#version 460 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normalVector;

layout (std140, binding=0) uniform VertexConstBuffer
{
    mat4 model;         // 0
    mat4 view;      // 64
    mat4 projection;    //  128
    mat4 invModel;  // 192
};

out VS_OUT {
  vec2 textureCoord;
} vs_out;

void main() {
  vs_out.textureCoord = texCoord;
  gl_Position = vec4(vertexPosition, 1);
}
)";

  thresholdShader = GaussianS::MakeShared();
  thresholdShader->SetShaderCode(ShaderType::VERTEX, vertexShader);
  thresholdShader->SetShaderCode(ShaderType::PIXEL, R"(
#version 460 core

uniform sampler2D texture0;

const float weights[7] = { 0.05, 0.1, 0.14, 0.6, 0.14, 0.1, 0.05};

layout (std140, binding=1) uniform PixelConstBuffer
{
  float dx;
  float dy;
  int space;
  float threshold;
};

out vec4 fragColor;
in VS_OUT {
  vec2 textureCoord;
} px_in;

void main() {
  vec4 color = texture(texture0, px_in.textureCoord);
  float l = (color.x + color.y +color.z) / 3.f;
  fragColor = l > threshold ? color : vec4(0,0,0,0);
}
)");
  thresholdShader->Initialize();

  combineShader = GaussianS::MakeShared();
  combineShader->SetShaderCode(ShaderType::VERTEX, vertexShader);
  combineShader->SetShaderCode(ShaderType::PIXEL, R"(
#version 460 core

uniform sampler2D texture0;
uniform sampler2D texture1;

layout (std140, binding=1) uniform PixelConstBuffer
{
  float dx;
  float dy;
  int space;
  float threshold;
};

out vec4 fragColor;
in VS_OUT {
  vec2 textureCoord;
} px_in;

void main() {
  fragColor = texture2D(texture0, px_in.textureCoord) + texture2D(texture1, px_in.textureCoord);
}
)");
  combineShader->Initialize();

  gaussianShader = GaussianS::MakeShared();
  gaussianShader->SetShaderCode(ShaderType::VERTEX, vertexShader);
  gaussianShader->SetShaderCode(ShaderType::PIXEL, R"(
#version 460 core

uniform sampler2D texture0;

const float weights[7] = { 0.05, 0.1, 0.14, 0.2, 0.14, 0.1, 0.05};

layout (std140, binding=1) uniform PixelConstBuffer
{
  float dx;
  float dy;
  int space;
  float threshold;
};

out vec4 fragColor;
in VS_OUT {
  vec2 textureCoord;
} px_in;

void main() {
  vec3 color = vec3(0,0,0);
  int i;
  for(i = 0; i < 7; ++i)
  {
    color += weights[i] * texture(texture0, px_in.textureCoord + vec2(dx , 0.0) * float((i - 3) * space)).xyz;
    color += weights[i] * texture(texture0, px_in.textureCoord + vec2(0.0 , dy) * float((i - 3) * space)).xyz;
  }
  fragColor = vec4(color, 1.f);
}
)");
  gaussianShader->Initialize();

  origin = GaussianF::MakeShared();
  origin->SetShader(thresholdShader);
  origin->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  origin->Initialize();
  t_filter = GaussianF::MakeShared();
  t_filter->SetShader(thresholdShader);
  t_filter->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  t_filter->Initialize();
  g_filter1 = GaussianF::MakeShared();
  g_filter1->SetShader(gaussianShader);
  g_filter1->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  g_filter1->Initialize();
  g_filter2 = GaussianF::MakeShared();
  g_filter2->SetShader(gaussianShader);
  g_filter2->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  g_filter2->Initialize();
  g_filter3 = GaussianF::MakeShared();
  g_filter3->SetShader(gaussianShader);
  g_filter3->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  g_filter3->Initialize();
  g_filter4 = GaussianF::MakeShared();
  g_filter4->SetShader(gaussianShader);
  g_filter4->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  g_filter4->Initialize();
  g_filter5 = GaussianF::MakeShared();
  g_filter5->SetShader(gaussianShader);
  g_filter5->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  g_filter5->Initialize();
  g_filter6 = GaussianF::MakeShared();
  g_filter6->SetShader(gaussianShader);
  g_filter6->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  g_filter6->Initialize();
  c_filter = GaussianF::MakeShared();
  c_filter->SetShader(combineShader);
  c_filter->SetScreenSize(glRenderer::instance()->m_option.width, glRenderer::instance()->m_option.height);
  c_filter->Initialize();

  origin->SetTextures({ glRenderer::instance()->m_textureBuffer });
  t_filter->SetTextures({ glRenderer::instance()->m_textureBuffer });
  g_filter1->SetTextures({ t_filter->m_textureBuffer });
  g_filter2->SetTextures({ g_filter1->m_textureBuffer });
  g_filter3->SetTextures({ g_filter2->m_textureBuffer });
  g_filter4->SetTextures({ g_filter3->m_textureBuffer });
  g_filter5->SetTextures({ g_filter4->m_textureBuffer });
  g_filter6->SetTextures({ g_filter5->m_textureBuffer });
  c_filter->SetTextures({ g_filter6->m_textureBuffer, origin->m_textureBuffer });
  c_filter->SetRenderTarget(0);

  thresholdShader->pixelConstBuffer->data.dx        = 1.f / glRenderer::instance()->m_option.width;
  thresholdShader->pixelConstBuffer->data.dy        = 1.f / glRenderer::instance()->m_option.height;
  thresholdShader->pixelConstBuffer->data.space     = 1;
  thresholdShader->pixelConstBuffer->data.threshold = 0.8f;

  gaussianShader->pixelConstBuffer->data.dx        = 1.f / glRenderer::instance()->m_option.width;
  gaussianShader->pixelConstBuffer->data.dy        = 1.f / glRenderer::instance()->m_option.height;
  gaussianShader->pixelConstBuffer->data.space     = 1;
  gaussianShader->pixelConstBuffer->data.threshold = 0.8f;

  combineShader->pixelConstBuffer->data.dx        = 1.f / glRenderer::instance()->m_option.width;
  combineShader->pixelConstBuffer->data.dy        = 1.f / glRenderer::instance()->m_option.height;
  combineShader->pixelConstBuffer->data.space     = 1;
  combineShader->pixelConstBuffer->data.threshold = 0.8f;
  Update();
}
void glBloomEffect::Update()
{
  thresholdShader->Bound();
  thresholdShader->pixelConstBuffer->Update();
  gaussianShader->Bound();
  gaussianShader->pixelConstBuffer->Update();
  combineShader->Bound();
  combineShader->pixelConstBuffer->Update();
}
void glBloomEffect::Render()
{
  thresholdShader->pixelConstBuffer->data.dx        = 1.f / glRenderer::instance()->m_option.width;
  thresholdShader->pixelConstBuffer->data.dy        = 1.f / glRenderer::instance()->m_option.height;
  thresholdShader->pixelConstBuffer->data.space     = 1;
  thresholdShader->pixelConstBuffer->data.threshold = 0.0f;
  thresholdShader->Bound();
  thresholdShader->pixelConstBuffer->Update();
  origin->Render();
  thresholdShader->pixelConstBuffer->data.dx        = 1.f / glRenderer::instance()->m_option.width;
  thresholdShader->pixelConstBuffer->data.dy        = 1.f / glRenderer::instance()->m_option.height;
  thresholdShader->pixelConstBuffer->data.space     = 1;
  thresholdShader->pixelConstBuffer->data.threshold = 0.8f;
  thresholdShader->Bound();
  thresholdShader->pixelConstBuffer->Update();
  t_filter->Render();
  // gaussianShader->pixelConstBuffer->data.dx        = 1.f / glRenderer::instance()->m_option.width;
  // gaussianShader->pixelConstBuffer->data.dy         = 1.f / glRenderer::instance()->m_option.height;
  // gaussianShader->pixelConstBuffer->data.space      = 10;
  // gaussianShader->pixelConstBuffer->data.threshold  = 0.8f;
  gaussianShader->Bound();
  gaussianShader->pixelConstBuffer->Update();
  g_filter1->Render();
  g_filter2->Render();
  g_filter3->Render();
  g_filter4->Render();
  g_filter5->Render();
  g_filter6->Render();
  c_filter->Render();
}
}  // namespace libEngine