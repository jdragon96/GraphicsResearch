#pragma once

#include "engine/Macro.h"
#include "imgui_impl_dx11.h"

class CGlobalPixel
{
public:
  Vec3  eyeWorld;
  float time = 0.f;
  Vec3  dummy;
  // float height      = 1.f;
  int mipmapLevel = 0;

  void Render()
  {
    ImGui::SetNextItemOpen(false, ImGuiCond_Once);
    if (ImGui::TreeNode("Global Pixel Parameter"))
    {
      ImGui::SliderFloat3("eyeWorld", &eyeWorld.x(), -100.f, 100.f);
      ImGui::InputFloat("time", &time);
      ImGui::SliderInt("mipmap level", &mipmapLevel, 0, 10);
      ////ImGui::SliderFloat("height", &height, 0.f, 2.f);
      ImGui::TreePop();
    }
  }
};