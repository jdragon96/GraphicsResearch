#include <libEngine/dx11/dxCubemap.h>
#include <algorithm>
#include <directxtk/DDSTextureLoader.h>  // 큐브맵 읽을 때 필요
#include <directxtk/WICTextureLoader.h>

namespace libEngine
{
dxCubemap::dxCubemap() : CubeMapBase()
{
}
dxCubemap::~dxCubemap()
{
}
void dxCubemap::Initialize()
{
  // 1. shader
  m_cubeShader = dxShaderBuffer<CubemapEffect::VertexConstBuf, CubemapEffect::PixelConstBuf>::MakeShared();
  m_cubeShader->SetShaderCode(ShaderType::VERTEX, CubemapEffect::dxVertexShaader);
  m_cubeShader->SetShaderCode(ShaderType::PIXEL, CubemapEffect::dxPixelShader);
  m_cubeShader->Initialize();

  // 2. Texture
  m_texture = dxTexture::MakeShared();
  for (auto& file : m_ddsList)
  {
    m_texture->AddImage(file);
  }

  // 3. mesh
  m_cubeMeshBuffer = dxMeshBuffer<CubemapEffect::VertexConstBuf, CubemapEffect::PixelConstBuf>::MakeShared();
  m_cubeMeshBuffer->SetShader(m_cubeShader);
  m_cubeMeshBuffer->SetMeshType(MeshType::TRIANGLE);
  auto cube = Toybox::MakeCube(100.f, 0.f, 0.f, 0.f);
  std::reverse(cube.indices.begin(), cube.indices.end());
  m_cubeMeshBuffer->SetMesh({ cube });
  m_cubeMeshBuffer->SetTexture(m_texture);
  m_cubeMeshBuffer->Initialize();
}

void dxCubemap::Bound()
{
  m_cubeShader->vertexConstBuffer->data.UpdateCamera(MemDB::CurrentCamera);
  m_cubeMeshBuffer->Render();
}
}  // namespace libEngine