#pragma once

#include "engine/Macro.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "imgui_impl_dx11.h"

class CCubemap
{
public:
  int useAlbedo   = 0;
  int useSpecular = 0;
  int useDiffuse  = 0;
  int dummy       = 0;

  void Render()
  {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Background"))
    {
      ImGui::CheckboxFlags("useAlbedo", &useAlbedo, 1);
      ImGui::CheckboxFlags("useSpecular", &useSpecular, 1);
      ImGui::CheckboxFlags("useDiffuse", &useDiffuse, 1);
      ImGui::TreePop();
    }
  }
};