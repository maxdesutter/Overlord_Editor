#include "OverlordEditorPCH.h"
#include "ObjectParameters.h"
#include "imgui.h"
#include "GameObject.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "TransformComponent.h"
#include "Material.h"
#include "ComponentFactory.h"
#include <DirectXCollision.h>
#include "Exposer.h"
#include "RigidBodyComponent.h"
#include "BehaviorComponentStructs.h"
#include "Components.h"

ObjectParameters::ObjectParameters()
{
	m_CompFactory = new ComponentFactory;
	m_Exposer = new Exposer();
}


ObjectParameters::~ObjectParameters()
{
	delete m_CompFactory;
	delete m_Exposer;
}

void ObjectParameters::Update(GameScene* scene, GameObject* Object)
{
	if (Object)
	{
		ParameterMenuBar(scene, Object);

		Rename(Object);


		ImGui::Text("Components");

		ImGui::PushTextWrapPos();
		std::vector<BaseComponent*>*allComponents = Object->GetAllComponents();
		TransformComponentPart(Object);
		ExposeRegisteredComponentVariables(allComponents, scene->GetGameContext());
		AddGameComponent(Object);
		if (m_RightClickedComponent)
			ShowPopUp();
		ImGui::PopTextWrapPos();
	}
}
void ObjectParameters::Rename(GameObject* Object)
{
	std::string name = std::string(Object->GetName().begin(), Object->GetName().end());
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
void ObjectParameters::ParameterMenuBar(GameScene* scene, GameObject* Object)
{

	if (ImGui::BeginMenu("Add New Component"))
	{
		//implement
		if (ImGui::MenuItem("Add ModelComponent"))
		{
			BaseComponent* mod = m_CompFactory->CreateModelComponent(dynamic_cast<EditorScene*>(scene)->GetModels()->at(0));
			Object->AddComponent(mod);
			mod->RunTimeInitialize(scene->GetGameContext());
		}
		if (ImGui::MenuItem("Add controllercomponent"))
		{

			

			BaseComponent* contr = m_CompFactory->CreateControllerComponent(*dynamic_cast<EditorScene*>(scene)->GetPhysxMaterials());
			Object->AddComponent(contr);
			contr->RunTimeInitialize(scene->GetGameContext());
		}
		if (ImGui::MenuItem("Add RigidBodyComponent"))
		{
			BaseComponent* rig = m_CompFactory->CreateRigidBodyComponent();
			Object->AddComponent(rig);
			rig->RunTimeInitialize(scene->GetGameContext());

			BaseComponent* col = m_CompFactory->CreateColliderComponent(*dynamic_cast<EditorScene*>(scene)->GetPhysxMaterials());
			Object->AddComponent(col);
			col->RunTimeInitialize(scene->GetGameContext());
		}
		if (ImGui::MenuItem("Add ColliderComponent"))
		{
			if (Object->HasComponent<RigidBodyComponent>())
			{
				BaseComponent* col = m_CompFactory->CreateColliderComponent(*dynamic_cast<EditorScene*>(scene)->GetPhysxMaterials());
				Object->AddComponent(col);
				col->RunTimeInitialize(scene->GetGameContext());
			}
		}
		if (ImGui::MenuItem("Add particleemmitercomponent"))
		{
			BaseComponent* col = m_CompFactory->CreateBaseParticleEmmiter(dynamic_cast<EditorScene*>(scene)->GetTextures()->at(0));
			Object->AddComponent(col);
			col->RunTimeInitialize(scene->GetGameContext());
		}
		ImGui::EndMenu();
	}
	ImGui::Separator();
}
void ObjectParameters::TransformComponentPart(GameObject* Object)
{
	ImGui::Separator();
	ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("TransFormComponent"))
	{
		TransformComponent* selectedTransform = dynamic_cast<TransformComponent*>(Object->GetAllComponents()->at(0));
		//Position
		{
			auto currentPos = selectedTransform->GetPosition();

			float posx, posy, posz;
			posx = currentPos.x;
			posy = currentPos.y;
			posz = currentPos.z;

			ImGui::PushID("Posx");
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("", &posx);
			ImGui::PopID();

			ImGui::SameLine();
			if (ImGui::IsItemClicked(1))
				posx = 0;


			ImGui::PushID("PosY");
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("", &posy);
			if (ImGui::IsItemClicked(1))
				posy = 0;
			ImGui::PopID();

			ImGui::PushID("PosZ");

			ImGui::SameLine();
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("Position", &posz);
			if (ImGui::IsItemClicked(1))
				posz = 0;
			ImGui::PopID();
			if (posx != currentPos.x || posy != currentPos.y || posz != currentPos.z)
			{
				selectedTransform->Translate(posx, posy, posz);
			}
		}
		//Scale
		{
			auto curentScale = selectedTransform->GetScale();

			float scaleX, scaleY, scaleZ;
			scaleX = curentScale.x;
			scaleY = curentScale.y;
			scaleZ = curentScale.z;

			ImGui::PushID("ScaleX");
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("", &scaleX, 0.1f);
			ImGui::SameLine();
			if (ImGui::IsItemClicked(1))
				scaleX = 1;
			ImGui::PopID();


			ImGui::PushID("ScaleY");
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("", &scaleY, 0.1f);
			if (ImGui::IsItemClicked(1))
				scaleY = 1;
			ImGui::PopID();

			ImGui::PushID("ScaleZ");
			ImGui::SameLine();
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("Scale", &scaleZ, 0.1f);
			if (ImGui::IsItemClicked(1))
				scaleZ = 1;
			ImGui::PopID();

			if (scaleX != curentScale.x || scaleY != curentScale.y || scaleZ != curentScale.z)
			{
				selectedTransform->Scale(scaleX, scaleY, scaleZ);
			}

		}
		//rotation
		{
			DirectX::XMFLOAT4 currentRot = selectedTransform->GetRotation();

			DirectX::XMVECTOR curQuat = DirectX::XMLoadFloat4(&currentRot);

			float roll = 0, origRoll, origRollDeg;
			float  pitch = 0, origPitch, origPitchDeg;
			float yaw = 0, origYaw, origYawDeg;

			origRoll = atan2(2 * currentRot.x*currentRot.w + 2 * currentRot.y*currentRot.z, 1 - 2 * currentRot.x*currentRot.x - 2 * currentRot.z*currentRot.z);
			origPitch = atan2(2 * currentRot.y*currentRot.w + 2 * currentRot.x*currentRot.z, 1 - 2 * currentRot.y*currentRot.y - 2 * currentRot.z*currentRot.z);
			origYaw = asin(2 * currentRot.x*currentRot.y + 2 * currentRot.z*currentRot.w);

			roll = origRollDeg = DirectX::XMConvertToDegrees(origRoll);
			pitch = origPitchDeg = DirectX::XMConvertToDegrees(origPitch);
			yaw = origYawDeg = DirectX::XMConvertToDegrees(origYaw);




			ImGui::PushID("rotRoll");
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("", &roll, 1, 0, 0, "%.1f");


			ImGui::PopID();

			ImGui::SameLine();
			if (ImGui::IsItemClicked(1))
				roll = 0;


			ImGui::PushID("rotPitch");
			ImGui::PushItemWidth(60);

			ImGui::DragFloat("", &pitch, 1, 0, 0, "%.1f");
			if (ImGui::IsItemClicked(1))
				pitch = 0;
			ImGui::PopID();

			ImGui::PushID("rotYaw");

			ImGui::SameLine();
			ImGui::PushItemWidth(60);
			ImGui::DragFloat("Rotation", &yaw, 1, 0, 0, "%.1f");
			if (ImGui::IsItemClicked(1))
				yaw = 0;
			ImGui::PopID();



			ImGuiIO io = ImGui::GetIO();
			if (io.WantTextInput)
			{
				TextInputRotation[3] = TextInputRotation[0];
				TextInputRotation[4] = TextInputRotation[1];
				TextInputRotation[5] = TextInputRotation[2];
				TextInputRotation[0] = roll;
				TextInputRotation[1] = pitch;
				TextInputRotation[2] = yaw;

				return;
			}

			if (TextInputRotation[3] != 0 || TextInputRotation[4] || TextInputRotation[5])
			{
				roll = TextInputRotation[3];
				pitch = TextInputRotation[4];
				yaw = TextInputRotation[5];
				TextInputRotation[0] = 0;
				TextInputRotation[1] = 0;
				TextInputRotation[2] = 0;
				TextInputRotation[3] = 0;
				TextInputRotation[4] = 0;
				TextInputRotation[5] = 0;
			}
			//rounding
			float difRoll = roundf((roll - origRollDeg) * 10) / 10;
			float difPitch = roundf((pitch - origPitchDeg) * 10) / 10;
			float difYaw = roundf((yaw - origYawDeg) * 10) / 10;

			float difRollr = roundf(std::abs(roll - origRollDeg) * 10) / 10;
			float difPitchr = roundf(std::abs(pitch - origPitchDeg) * 10) / 10;
			float difYawr = roundf(std::abs(yaw - origYawDeg) * 10) / 10;





			if (difRollr || difPitchr || difYawr)
			{

				DirectX::XMVECTOR res = DirectX::XMQuaternionMultiply(curQuat,
					DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(difRoll),
						DirectX::XMConvertToRadians(difPitch), DirectX::XMConvertToRadians(difYaw)));
				selectedTransform->Rotate(res);
			}
			if (std::isnan(difRollr) || std::isnan(difPitchr) || std::isnan(difYawr))
			{
				selectedTransform->Rotate(0, 0, 90);

			}

		}
	}
}
void ObjectParameters::ExposeRegisteredComponentVariables(std::vector<BaseComponent*>* allComponents, const GameContext& gameContext)
{
	for (SIZE_T index = 1; index < allComponents->size(); index++)
	{
		ImGui::PushID(index);
		ImGui::Separator();
		ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader(allComponents->at(index)->GetComponentClassName().c_str()))
		{
			if (ShouldShowPopUp())
			{
				m_RightClickedComponent = allComponents->at(index);
				ImGui::PopID();

				continue;
			}

			std::vector<GetSetinterface*>* methods = allComponents->at(index)->GetRegisteredMethods();
			m_Exposer->ExposeMethods(methods, gameContext);
		
		}
		ImGui::PopID();

	}
}
void ObjectParameters::AddGameComponent(GameObject* Object)
{
	ImGui::NewLine();
	ImGui::NewLine();

	ImGui::Separator();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
	ImGui::Button("BHVIOR Component", ImVec2(100, 60));
	ImGui::PopStyleColor();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_Behavior_Component"))
		{
			IM_ASSERT(payload->DataSize == sizeof(BaseBehaviorComponentStruct*));
			BaseBehaviorComponentStruct* payloaddata = *(BaseBehaviorComponentStruct**)payload->Data;

			Object->AddComponent((BaseComponent*)payloaddata->Getnew());
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	ImGui::PushTextWrapPos();
	ImGui::Text("Drag to add new behavior component");
	ImGui::PopTextWrapPos();
}
bool ObjectParameters::ShouldShowPopUp()
{
	if (ImGui::IsItemClicked(1))
	{
		return true;
	}
	return false;
}
void ObjectParameters::ShowPopUp()
{
	ImGui::OpenPopup("Options");
	if (ImGui::BeginPopupModal("Options", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{


		if (ImGui::Button("DELETE", ImVec2(120, 0)))
			ImGui::OpenPopup("delete");

		if (ImGui::BeginPopupModal("delete"))
		{
			ImGui::Text("Are you shure about that. \n This cannot be undone");

			if (ImGui::Button("Confirm"))
			{
				m_RightClickedComponent->GetGameObject()->RemoveComponent(m_RightClickedComponent, true);
				m_RightClickedComponent = nullptr;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("cancel"))
			{
				ImGui::CloseCurrentPopup();

			}
			ImGui::EndPopup();
		}


		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_RightClickedComponent = nullptr;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}