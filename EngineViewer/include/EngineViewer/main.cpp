#include <vector>

#include "stb_image.h"
#include "libEngine/utils/toybox.h"
#include "libEngine/API.h"
#include "libEngine/dx11/dxMemoryDB.h"
#include "libEngine/opengl/glShaderBuffer.h"
#include "libEngine/model/BlinnPhongEffect.h"
#include <type_traits>
#include "libEngine/dx11/dxCubemap.h"
#include "libEngine/opengl/glCubemap.h"
#include "libEngine/opengl/glRenderer.h"
#include "libEngine/dx11/dxMeshBuffer.h"
#include "libEngine/shared/MeshBufferBase.h"
#include "libEngine/dx11/dxFilter.h"

#include "libEngine/opengl/postprocessing/glBloomEffect.h"
#include "libEngine/dx11/postprocessing/dxBloomEffect.h"

using namespace libEngine;

int main()
{
  CameraOption camOpt;
  camOpt.sensitivity = 0.001f;
  camOpt.SetPerspective(45, 1280.f / 800.f, 0.01f, 1000.f);
  camOpt.SetView(Vec3(-10, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, 1));
  auto defaultCamera = API::instance().MakeCamera(camOpt);
  defaultCamera->Initialize();

  MemDB::SetEngine(EngineType::DX11);
  MemDB::SetCamera(defaultCamera);

  RendererOption opt;
  opt.width  = 1280;
  opt.height = 800;
  opt.title  = "Test Window";

  auto renderer = API::instance().MakeRenderer(opt);
  renderer->Initialize();

  auto texture = API::instance().MakeTexture();
  {
    {
      texture->AddImage("./Resource/image/earth.jpg");
      texture->AddDDS("./Resource/image/skybox/Atrium_diffuseIBL.dds");
      texture->AddDDS("./Resource/image/skybox/Atrium_specularIBL.dds");
    }
    {
      // texture->AddImage("./Resource/image/earth.jpg");
      // texture->AddCubemap({ "./Resource/image/skybox/right.jpg", "./Resource/image/skybox/left.jpg",
      //                       "./Resource/image/skybox/bottom.jpg", "./Resource/image/skybox/top.jpg",
      //                       "./Resource/image/skybox/front.jpg", "./Resource/image/skybox/back.jpg" });
    }
  }

  auto cubemap = API::instance().MakeCubeMap();
  {
    {
      cubemap->LoadDDS({ "./Resource/image/skybox/Atrium_diffuseIBL.dds" });
      cubemap->LoadDDS({ "./Resource/image/skybox/Atrium_specularIBL.dds" });
    }
    {
      // cubemap->LoadPictures({ { "./Resource/image/skybox/right.jpg", "./Resource/image/skybox/left.jpg",
      //                           "./Resource/image/skybox/bottom.jpg", "./Resource/image/skybox/top.jpg",
      //                           "./Resource/image/skybox/bottom.jpg", "./Resource/image/skybox/top.jpg",
      //                           "./Resource/image/skybox/front.jpg", "./Resource/image/skybox/back.jpg" } });
    }
  }
  cubemap->Initialize();

  auto defaultShader = API::instance().MakeShader<>();
  {
    {
      // defaultShader->SetShaderPath(ShaderType::VERTEX, "./Resource/glColorShader.vert");
      // defaultShader->SetShaderPath(ShaderType::PIXEL, "./Resource/glColorShader.frag");
    }
    {
      defaultShader->SetShaderPath(ShaderType::VERTEX, "./Resource/dxColorVertexShader.hlsl");
      defaultShader->SetShaderPath(ShaderType::PIXEL, "./Resource/dxColorPixelShader.hlsl");
    }
  }
  defaultShader->Initialize();

  auto meshData = Toybox::ReadObject("./Resource/object/stanford_dragon.stl");
  auto buffer   = API::instance().MakeMeshBuffer<>({ Toybox::MakeSphere(1, 1, 1, 1, 50, 50) });
  // auto buffer = API::instance().MakeMeshBuffer<>({ Toybox::MakeSuqare() });
  buffer->SetTexture(texture);
  buffer->SetShader(defaultShader);
  buffer->Move(Vec3(0, 0, 0));
  buffer->SetNormalRenderFlag(false);
  buffer->Initialize();

  // auto dragonNormalBuffer = API::instance().MakeMeshBuffer(meshData);
  // dragonNormalBuffer->SetShader(normalShader);
  // dragonNormalBuffer->Move(Vec3(0, 0, 0));
  // dragonNormalBuffer->Initialize();

  BlinnPhong::Light light;
  light.direction = Vec3(0.f, 0.f, -1.f);
  light.position  = Vec3(0.f, 0.f, 20.f);

  BlinnPhong::Material dragonMat;
  dragonMat.ambient        = Vec3(0.5f, 0.1f, 0.1f);
  dragonMat.ambientFactor  = 1.f;
  dragonMat.diffuse        = Vec3(1.f, 1.f, 1.f);
  dragonMat.shininess      = 1.f;
  dragonMat.specular       = Vec3(1.f, 1.f, 1.f);
  dragonMat.specularFactor = 0.3f;

  defaultShader->pixelConstBuffer->data.useTexture = false;
  defaultShader->pixelConstBuffer->data.light      = light;
  defaultShader->pixelConstBuffer->data.material   = dragonMat;

  auto bloomEft = API::instance().MakeBloomEffect();
  bloomEft->Initialize();

  renderer->SetPrevRenderFunc([&]() { renderer->SetCamera(defaultCamera); });
  renderer->SetRenderFunc([&]() {
    defaultShader->vertexConstBuffer->data.UpdateCamera(defaultCamera);
    defaultShader->vertexConstBuffer->data.UpdateCamera(defaultCamera);
    defaultShader->pixelConstBuffer->data.UpdateCamera(defaultCamera);
    defaultShader->Bound();
    defaultShader->pixelConstBuffer->Update();
    cubemap->Bound();
    buffer->Render();
  });
  renderer->SetPostProcessingFunc([&]() {
    //
    bloomEft->Render();
  });
  renderer->Run();
}