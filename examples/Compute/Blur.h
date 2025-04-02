#pragma once

#include "engine/dx11/Dx11EngineManager.h"
#include "engine/dx11/Dx11GraphicsPSO.h"
#include "engine/dx11/Dx11MeshBuffer.h"
#include "engine/dx11/Dx11ConstantBuffer.h"
#include "engine/dx11/Dx11Filter.h"
#include "engine/common/CameraBuffer.h"
#include "engine/common/Drawing.h"
#include "engine/model/CBlinnPhong.h"
#include "engine/model/CCommon.h"
#include "engine/model/CBillboardPoint.h"
#include "engine/model/CBillboardCube.h"
#include "engine/model/CGlobalPixel.h"
#include "engine/model/CTextures.h"
#include "engine/dx11/Dx11EnginePSO.h"
#include "engine/dx11/Dx11ComputeBuffer.h"

class CircleConstBuffer
{
public:
  Vec3  dummy1;
  float scale = 1.f;

  void Render()
  {
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Compute"))
    {
      ImGui::SliderFloat("scale", &scale, 0.f, 1.f);
      ImGui::TreePop();
    }
  }
};

class BlurConstBuffer
{
public:
  Vec3 dummy1;
  int  useHorizontal;
};

void CircleExample();
