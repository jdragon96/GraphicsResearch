#pragma once

#include "engine/Macro.h"

class CCommon
{
public:
  Mat4  view;
  Mat4  projection;
  Mat4  invProj;
  Vec3  dummy;
  float heightScale = 1.f;

  void Render()
  {
    ImGui::SetNextItemOpen(false, ImGuiCond_Once);
    if (ImGui::TreeNode("Global Vertex Parameter"))
    {
      ImGui::SliderFloat("height scale", &heightScale, 0.f, 2.f);
      ImGui::TreePop();
    }
  }
};