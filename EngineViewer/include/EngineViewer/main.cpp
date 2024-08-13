#include <vector>
#include "libEngine/libEngine.h"

#include "libEngine/Workspace.h"

using namespace libEngine;

int main()
{
  Workspace::instance().SetEngine(EngineType::OPENGL);

  RendererOption opt;
  opt.width  = 1280;
  opt.height = 800;
  opt.title  = "Test Window";

  auto renderer = libEngine::Workspace::instance().MakeRenderer(opt);
  renderer->Initialize();

  CameraOption camOpt;
  camOpt.SetPerspective(45, 1280.f / 800.f, 0.01f, 100.f);
  camOpt.SetView(Vec3(2, 0, 0), Vec3(0, 0, 0), Vec3(0, 0, 1));
  auto defaultCamera = Workspace::instance().MakeCamera(camOpt);
  defaultCamera->Initialize();

  auto defaultShader = Workspace::instance().MakeShader();
  defaultShader->SetVertexShader("./Resource/colorShader.vert");
  defaultShader->SetPixelShader("./Resource/colorShader.frag");
  //defaultShader->SetVertexShader("./Resource/colorShader.vertex.hlsl");
  //defaultShader->SetPixelShader("./Resource/colorShader.pixel.hlsl");
  defaultShader->initialize();

  auto buffer = Workspace::instance().MakeMeshBuffer({ Toybox::MakeCube(0.5f, 1, 0, 0) });
  buffer->SetShader(defaultShader);
  buffer->Initialize();

  renderer->SetPrevRenderFunc([]() {});
  renderer->SetRenderFunc([&]() {
    defaultShader->Bound();
    defaultShader->UpdateMat4("view", defaultCamera->GetViewMatPtr());
    defaultShader->UpdateMat4("projection", defaultCamera->GetProjMatPtr());
    buffer->Render();
  });
  renderer->Run();
}