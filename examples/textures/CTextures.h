#pragma once

#include "engine/Macro.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "imgui_impl_dx11.h"

class CTextures
{
public:
  Vec3 dummy;
  int  mipmapLevel = 0;

  void Render()
  {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Texture"))
    {
      ImGui::SliderInt("Mipmap Level", &mipmapLevel, 0, 10);
      ImGui::TreePop();
    }
  }
};