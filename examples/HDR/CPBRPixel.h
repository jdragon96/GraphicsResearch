#pragma once

#include "engine/Macro.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "imgui_impl_dx11.h"

class CPBRPixel
{
public:
  int useAlbedoMap;
  int useAOMap;
  int useMetallicMap;
  int useRoughnessMap;

  void Render()
  {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("PBR Texture"))
    {
      ImGui::CheckboxFlags("useAlbedoMap", &useAlbedoMap, 1);
      ImGui::CheckboxFlags("useAOMap", &useAOMap, 1);
      ImGui::CheckboxFlags("useMetallicMap", &useMetallicMap, 1);
      ImGui::CheckboxFlags("useRoughnessMap", &useRoughnessMap, 1);
      ImGui::TreePop();
    }
  }
};