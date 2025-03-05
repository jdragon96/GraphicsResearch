#pragma once

#include "engine/Macro.h"
#include "imgui_impl_dx11.h"
//#include "imgui_impl_opengl3.h"

struct Light
{
	Vec3  direction;
	float dummy1;
	Vec3  position;
	float dummy2;
};

struct Material
{
	Vec3  ambient;
	float ambientFactor;
	Vec3  diffuse;
	float shininess;
	Vec3  specular;
	float specularFactor;
};

class CBlinnPhong
{
public:
	Light    light;
	Material mat;
	Vec3     eyeWorld;
	float    dummy1;
	int      useBP;
	int      useAmbient = 0;
  int      useDiffuse = 0;
  int      useTexture = 0;

	void Render()
	{
		ImGui::SetNextItemOpen(false, ImGuiCond_Once);
		ImGui::SliderFloat3("eyeWorld", &eyeWorld.x(), -100.f, 100.f);
		if (ImGui::TreeNode("Light"))
		{
			ImGui::SliderFloat3("Position", &light.position.x(), -10.f, 10.f);
			ImGui::SliderFloat3("Direction", &light.direction.x(), -1.f, 1.f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Material"))
		{
			ImGui::SliderFloat3("ambient", &mat.ambient.x(), 0.f, 1.f);
			ImGui::SliderFloat("ambientFactor", &mat.ambientFactor, 0.f, 10.f);
			ImGui::SliderFloat3("diffuse", &mat.diffuse.x(), 0.f, 1.f);
			ImGui::SliderFloat("shininess", &mat.shininess, 0.f, 10.f);
			ImGui::SliderFloat3("specular", &mat.specular.x(), 0.f, 1.f);
			ImGui::SliderFloat("specularFactor", &mat.specularFactor, 0.f, 10.f);
			ImGui::CheckboxFlags("useBP", &useBP, 1);
			ImGui::CheckboxFlags("useAmbient", &useAmbient, 1);
			ImGui::CheckboxFlags("useDiffuse", &useDiffuse, 1);
      ImGui::CheckboxFlags("useTexture", &useTexture, 1);
			ImGui::TreePop();
		}
	}
};