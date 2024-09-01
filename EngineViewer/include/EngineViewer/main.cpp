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
  texture->AddImage("./Resource/image/earth.jpg");
  // texture->AddCubemap({ "./Resource/image/skybox/right.jpg", "./Resource/image/skybox/left.jpg",
  //                       "./Resource/image/skybox/bottom.jpg", "./Resource/image/skybox/top.jpg",
  //                       "./Resource/image/skybox/front.jpg", "./Resource/image/skybox/back.jpg" });
  texture->AddDDS("./Resource/image/skybox/skybox.dds");

  auto cubemap = API::instance().MakeCubeMap();
  cubemap->LoadDDS({ "./Resource/image/skybox/skybox.dds" });
  // cubemap->LoadPictures({ { "./Resource/image/skybox/right.jpg", "./Resource/image/skybox/left.jpg",
  //                           "./Resource/image/skybox/bottom.jpg", "./Resource/image/skybox/top.jpg",
  //                           "./Resource/image/skybox/front.jpg", "./Resource/image/skybox/back.jpg" } });
  cubemap->Initialize();

  auto defaultShader = API::instance().MakeShader();
  // defaultShader->SetShaderPath(ShaderType::VERTEX, "./Resource/glColorShader.vert");
  // defaultShader->SetShaderPath(ShaderType::PIXEL, "./Resource/glColorShader.frag");
  defaultShader->SetShaderPath(ShaderType::VERTEX, "./Resource/dxColorVertexShader.hlsl");
  defaultShader->SetShaderPath(ShaderType::PIXEL, "./Resource/dxColorPixelShader.hlsl");
  defaultShader->Initialize();

  // auto buffer   = API::instance().MakeMeshBuffer({ Toybox::MakeSphere(0.5f, 1, 0, 0) });
  auto meshData = Toybox::ReadObject("./Resource/object/stanford_dragon.stl");
  auto buffer   = API::instance().MakeMeshBuffer(meshData);
  buffer->SetTexture(texture);
  buffer->SetShader(defaultShader);
  buffer->Move(Vec3(0, 0, 0));
  buffer->SetNormalRenderFlag(true);
  buffer->Initialize();
  // auto dragonNormalBuffer = API::instance().MakeMeshBuffer(meshData);
  // dragonNormalBuffer->SetShader(normalShader);
  // dragonNormalBuffer->Move(Vec3(0, 0, 0));
  // dragonNormalBuffer->Initialize();

  auto sunBuffer = API::instance().MakeMeshBuffer({ Toybox::MakeSphere(3, 1, 0, 0) });
  sunBuffer->SetShader(defaultShader);
  sunBuffer->Move(Vec3(10, 0, 30));
  sunBuffer->Initialize();

  auto sunBuffer2 = API::instance().MakeMeshBuffer({ Toybox::MakeSphere(3, 0, 1, 0) });
  sunBuffer2->SetShader(defaultShader);
  sunBuffer2->Move(Vec3(0, 0, 20.f));
  sunBuffer2->Initialize();

  BlinnPhong::Light light;
  light.direction = Vec3(0.f, 0.f, -1.f);
  light.position  = Vec3(0.f, 0.f, 20.f);

  // l.
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

  renderer->SetPrevRenderFunc([&]() { renderer->SetCamera(defaultCamera); });
  renderer->SetRenderFunc([&]() {
    defaultShader->vertexConstBuffer->data.UpdateCamera(defaultCamera);
    defaultShader->pixelConstBuffer->data.UpdateCamera(defaultCamera);
    defaultShader->Bound();
    defaultShader->pixelConstBuffer->Update();
    buffer->Render();
    cubemap->Bound();
  });
  renderer->Run();
}