#include "OverlordEditorPCH.h"
#include "MaterialParameters.h"
#include "imgui.h"
#include "Material.h"
#include "Exposer.h"
#include "GameScene.h"
MaterialParameters::MaterialParameters()
{
	m_Exposer = new Exposer();
}


MaterialParameters::~MaterialParameters()
{
	delete m_Exposer;
}

void MaterialParameters::UpdateMaterials(GameScene* scene, Material* mat)
{
	if (mat)
	{
		RenameMaterial(mat);
		ExposeParamterars(scene, mat);
	}
}

void MaterialParameters::UpdatePhysxMaterials(GameScene* scene, PhysicsMaterialStruct* mat)
{
	if (mat)
	{
		RenamePhysicMaterial(mat);

		ImGui::PushItemWidth(60);
		physx::PxReal dyn = mat->material->getDynamicFriction(),dynori = dyn;
		physx::PxReal stat = mat->material->getStaticFriction(),statori=stat;
		physx::PxReal rest = mat->material->getRestitution(),restori=rest;

		ImGui::DragFloat("Static Friction", &stat);
		ImGui::DragFloat("Dynamic Friction", &dyn);
		ImGui::DragFloat("restition", &rest);

		if(dyn!= dynori)
		{
			
			mat->material->setDynamicFriction(dyn);
		}
		if (stat != statori)
		{
			mat->material->setStaticFriction(stat);
		}
		if (rest != restori)
		{
			if (rest < 0)
				rest = 0;
			mat->material->setRestitution(rest);
		}
		ImGui::PopItemWidth();
	}
}

void MaterialParameters::RenameMaterial(Material* Object)
{
	std::wstring wname = Object->GetName();
	std::string name = std::string(wname.begin(), wname.end());
	char* before = new char[100], *after = new char[100];

	strcpy_s(before, name.length() + 1, name.c_str());
	strcpy_s(after, name.length() + 1, name.c_str());

	if (ImGui::InputText("Name", after, 100, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (std::string(before) != std::string(after))
		{
			std::string afterstr = std::string(after);
			std::wstring afterwstr = std::wstring(afterstr.begin(), afterstr.end());
			Object->SetName(afterwstr);
		}
	}

	delete[]before;
	delete[]after;


	ImGui::Separator();
}

void MaterialParameters::RenamePhysicMaterial(PhysicsMaterialStruct* mat)
{
	std::wstring wname = mat->materialName;
	std::string name = std::string(wname.begin(), wname.end());
	char* before = new char[100], *after = new char[100];

	strcpy_s(before, name.length() + 1, name.c_str());
	strcpy_s(after, name.length() + 1, name.c_str());

	if (ImGui::InputText("Name", after, 100, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (std::string(before) != std::string(after))
		{
			std::string afterstr = std::string(after);
			std::wstring afterwstr = std::wstring(afterstr.begin(), afterstr.end());
			mat->materialName =afterwstr;
		}
	}

	delete[]before;
	delete[]after;


	ImGui::Separator();
}

void MaterialParameters::ExposeParamterars(GameScene* scene, Material* mat)
{
	std::wstring wname = mat->GetTypeName();
	std::string name(wname.begin(), wname.end());

		ImGui::Separator();
		ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			std::vector<GetSetinterface*>* methods = mat->GetRegisteredMethods();
			m_Exposer->ExposeMethods(methods, scene->GetGameContext());

		}

	
}
