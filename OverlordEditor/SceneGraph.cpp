#include "OverlordEditorPCH.h"
#include "SceneGraph.h"
#include "GameScene.h"
#include "GameObject.h"
#include "imgui.h"
#include "TransformComponent.h"
#include "Components.h"

SceneGraph::SceneGraph()
{
}


SceneGraph::~SceneGraph()
{
}

void SceneGraph::Update(GameScene* scene)
{
	createGraph(scene);


}
void SceneGraph::createGraph(GameScene* scene)
{
	//create window
	ImGui::SetNextWindowPos({ 0,20 }, ImGuiSetCond_Once);
	ImGui::SetNextWindowSize({ 300,880 }, ImGuiCond_Once);
	ImGui::Begin("Scene Graph", 0, { ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize });


	int noNameIter = 0;


	int gameObjectItertor = 1;

	ImGui::SetNextTreeNodeOpen(true);
	bool node_open = ImGui::TreeNodeEx("Scene");
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SceneGraph"))
		{
			IM_ASSERT(payload->DataSize == sizeof(GameObject**));
			GameObject* payloaddata = *(GameObject**)payload->Data;

			GameObject* parent = payloaddata->GetParent();
			if (parent)
			{
				auto pos = payloaddata->GetTransform()->GetWorldPosition();
				auto rot = payloaddata->GetTransform()->GetWorldRotation();
				auto scale = payloaddata->GetTransform()->GetWorldScale();

				parent->RemoveChild(payloaddata);
				scene->AddChild(payloaddata);
				payloaddata->GetTransform()->Translate(pos);
				DirectX::XMVECTOR rotvec = DirectX::XMLoadFloat4(&rot);
				payloaddata->GetTransform()->Rotate(rotvec);
				payloaddata->GetTransform()->Scale(scale);
			}
		}
	}
	if (node_open)
	{
		for (auto obj : scene->GetChildren())
		{
			DisplayGameObject(obj, gameObjectItertor, noNameIter);
		}
		if (m_SelectedSceneGraphNode != -1)
		{
			m_Selection_mask_SceneGraph = (1 << m_SelectedSceneGraphNode);           // Click to single-select
		}
		else
		{
			m_Selection_mask_SceneGraph = 0;
		}

		ImGui::TreePop();
	}
	if (InputManager::IsMouseButtonDown(VK_RBUTTON) && ImGui::IsMouseHoveringWindow())
	{
		m_pSelectedObject = nullptr;
		m_SelectedSceneGraphNode = -1;

	}
	ImGui::End();

}
void SceneGraph::DisplayGameObject(GameObject* obj, int& index, int& noNameIter)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((m_Selection_mask_SceneGraph & (1 << index)) ? ImGuiTreeNodeFlags_Selected : 0);

	//Get name of object
	std::string name(obj->GetName().begin(), obj->GetName().end());
	if (name == "")
	{
		name = "Game Object :" + std::to_string(noNameIter);
		noNameIter++;
	}
	//check if object has children to make tree or leaf
	if (obj->GetChildren<GameObject>().size())
	{
		//has children
		//make tree node
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)index, node_flags, name.c_str(), index);
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			void* vobj = obj;
			ImGui::SetDragDropPayload("DND_SceneGraph", &vobj, sizeof(GameObject**));        // Set payload to carry the index of our item (could be anything)
			ImGui::Text("Place %s material", name.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SceneGraph"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject**));
				GameObject* payloaddata = *(GameObject**)payload->Data;



				GameObject* parent = payloaddata->GetParent();
				if (parent)
				{
					payloaddata->GetParent()->RemoveChild(payloaddata);
				}
				else
				{
					payloaddata->GetScene()->RemoveChild(payloaddata, false);
				}
				
				obj->AddChild(payloaddata);


			}
			ImGui::EndDragDropTarget();
		}


		//check if item was selected with mouse
		if(m_pSelectedObject==obj)
		{
			m_SelectedSceneGraphNode = index;

		}


		//if node is clicked
		if (ImGui::IsItemClicked())
		{
			m_pSelectedObject = obj;
			m_SelectedSceneGraphNode = index;
		}
		index++;

		//go over children
		if (node_open)
		{
			for (auto gObj : obj->GetChildren<GameObject>())
			{
				DisplayGameObject(gObj, index, noNameIter);
			}
			ImGui::TreePop();

		}
	}
	else
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		//has no children
		ImGui::TreeNodeEx((void*)(intptr_t)index, node_flags, name.c_str(), index);
		//check if item was selected with mouse
		if (m_pSelectedObject == obj)
		{
			m_SelectedSceneGraphNode = index;

		}
		if (ImGui::IsItemClicked())
		{
			m_SelectedSceneGraphNode = index;
			m_pSelectedObject = obj;
		}
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			void* vobj = obj;
			ImGui::SetDragDropPayload("DND_SceneGraph", &vobj, sizeof(GameObject**));        // Set payload to carry the index of our item (could be anything)
			ImGui::Text("Place %s material", name.c_str());
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SceneGraph"))
			{
				IM_ASSERT(payload->DataSize == sizeof(GameObject**));
				GameObject* payloaddata = *(GameObject**)payload->Data;

				GameObject* parent = payloaddata->GetParent();
				if (obj->GetParent() != payloaddata)
				{
					if (parent)
					{


						payloaddata->GetParent()->RemoveChild(payloaddata);
					}
					else
					{
						payloaddata->GetScene()->RemoveChild(payloaddata, false);
					}
					obj->AddChild(payloaddata);


				}
			}
			ImGui::EndDragDropTarget();
		}


		index++;
	}
}
