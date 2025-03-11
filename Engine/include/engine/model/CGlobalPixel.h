#pragma once

#include "engine/Macro.h"
#include "imgui_impl_dx11.h"

class CGlobalPixel
{
public:
  Vec3  eyeWorld;
  float time = 0.f;

  void Render()
  {
    ImGui::SetNextItemOpen(false, ImGuiCond_Once);
    if (ImGui::TreeNode("Light"))
    {
      ImGui::SliderFloat3("eyeWorld", &eyeWorld.x(), -100.f, 100.f);
      ImGui::InputFloat("time", &time);
      ImGui::TreePop();
    }
  }
};