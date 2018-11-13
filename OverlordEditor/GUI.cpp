#include "OverlordEditorPCH.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "GUI.h"




#include "Components.h"
#include "GameObject.h"
#include "Material.h"

#include <filesystem>

//subclasses
#include "ContentBrowser.h"
#include "SceneGraph.h"
#include "ParameterWindow.h"
#include "GameObjectFactory.h"
#include "imgui_internal.h"
#include "OverlordGame.h"
#include "DebugRenderer.h"

GUI::GUI()
{
	m_PContentBrowser = new ContentBrowser();
	m_pSceneGraph = new SceneGraph();
	m_pParameterWindow = new ParameterWindow();
	m_Creator = new GameObjectFactory();
}
GUI::~GUI()
{
	delete m_PContentBrowser;
	delete m_pSceneGraph;
	delete m_pParameterWindow;
	delete m_Creator;
}
void GUI::Update(GameScene* scene)
{
	//ImGui::ShowDemoWindow(&m_Demo);
	MainMenuBar(scene);
	if(m_Rename)
	{
		RenameLevel(scene);
	}


	//picking
	ImGuiIO io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		if (InputManager::IsMouseButtonDown(VK_LBUTTON) && !InputManager::IsMouseButtonDown(VK_LBUTTON, true))
			Picking(scene);
	}

	//selection
	m_pSceneGraph->Update(scene);
	m_SelectedObject = m_pSceneGraph->GetSelectedObject();
	if(m_SelectedObject && m_lastSet!=0)
	{
		m_lastSet = 0;
		m_PContentBrowser->Deselect();
	}

	m_pSelectedMaterial = m_PContentBrowser->GetSelectedMaterial();
	if(m_pSelectedMaterial && m_lastSet != 1)
	{
		m_lastSet = 1;
		m_pSceneGraph->DeselectGameObject();
	}
	m_pSelectedPhysxMat = m_PContentBrowser->GetSelectedPhysxMaterial();
	if(m_pSelectedPhysxMat && m_lastSet !=2)
	{
		m_lastSet = 2;
		m_pSceneGraph->DeselectGameObject();
	}
	m_pParameterWindow->Update(scene, m_SelectedObject, m_pSelectedMaterial, m_pSelectedPhysxMat);
	m_PContentBrowser->Update(scene);
	DrawBoundingBox();


	auto payload =ImGui::GetDragDropPayload();

	if(payload && ImGui::IsMouseReleased(0))
	{
		if((std::string)payload->DataType == "DND_Material")
		{
			auto obj = PickGameObject(scene);
			if(obj)
			{
				auto modelsc = obj->GetComponents<ModelComponent>();
				for(auto model :modelsc)
				{
					IM_ASSERT(payload->DataSize == sizeof(UINT));
					UINT payloaddata = *(UINT*)payload->Data;
					auto methods =  model->GetRegisteredMethods();
					for (auto method:*methods)
					{
						if(method->type==TYPE_MATERIAL)
						{
							GetSetMethods<ReflectionMaterialStruct>* methodStruct = ((GetSetMethods<ReflectionMaterialStruct>*)method);
							methodStruct->Set(payloaddata);
						}
					}
				}
			}
		}
	}

}



void GUI::Picking(GameScene* scene)
{
	m_pSceneGraph->SetObjectPicking(PickGameObject(scene));

	

}

GameObject* GUI::PickGameObject(GameScene* scene)
{
	//make ray
	DirectX::XMFLOAT3 nearPoint, farPoint, direction;
	DirectX::XMFLOAT2 ndc;

	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	const auto mousePosition = InputManager::GetMousePosition();

	const int width = windowSettings.Width / 2;
	const int height = windowSettings.Height / 2;

	ndc.x = (static_cast<float>(mousePosition.x) - width) / width;
	ndc.y = (height - static_cast<float>(mousePosition.y)) / height;

	auto vpi = scene->GetGameContext().pCamera->GetViewProjectionInverse();

	XMStoreFloat3(&nearPoint, DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(ndc.x, ndc.y, 0, 0), DirectX::XMLoadFloat4x4(&vpi)));
	XMStoreFloat3(&farPoint, DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(ndc.x, ndc.y, 1, 0), DirectX::XMLoadFloat4x4(&vpi)));

	direction.x = farPoint.x - nearPoint.x;
	direction.y = farPoint.y - nearPoint.y;
	direction.z = farPoint.z - nearPoint.z;


	DirectX::XMVECTOR directionv, origingv;
	directionv = DirectX::XMLoadFloat3(&direction);
	origingv = DirectX::XMLoadFloat3(&nearPoint);

	directionv = DirectX::XMVector3Normalize(directionv);
	//check model components
	GameObject* returningObj = nullptr;
	float closest = FLT_MAX;
	for (auto obj : scene->GetChildren())
	{
		auto models = obj->GetComponents<ModelComponent>();
		for (auto model : models)
		{

			float distance = (model->Intersects(origingv, directionv, 1000000));
			if (distance != 0 && distance < closest)
			{
				closest = distance;
				returningObj = obj;
			}
		}
		auto inchild = PickGameObjectInChildren(obj, directionv,origingv,&closest);
		if(inchild)
		{
			returningObj = inchild;
		}
	}
	return returningObj;
}

GameObject* GUI::PickGameObjectInChildren(GameObject* obj, DirectX::XMVECTOR directio, DirectX::XMVECTOR origin, float* foundDistanse)
{
	GameObject* returningObj = nullptr;
	for (auto objc : obj->GetChildren<GameObject>())
	{
		auto modelsc = objc->GetComponents<ModelComponent>();
		for (auto modelc : modelsc)
		{

			float distance = (modelc->Intersects(origin, directio, 1000000));
			if (distance != 0 && distance < *foundDistanse)
			{
				*foundDistanse = distance;
				returningObj = objc;
			}
		}
		auto inchild =PickGameObjectInChildren(objc, directio,origin, foundDistanse);
		if (inchild)
			returningObj = inchild;
	}
	return returningObj;
}

void GUI::MainMenuBar(GameScene* scene)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Load From File"))
			{
				LoadFromFile(scene);
				ImGui::EndMenu();
			}
			if(ImGui::MenuItem("Save To File"))
			{
				EditorScene* eScene = dynamic_cast<EditorScene*>(scene);
				eScene->SaveToFile(L"./Files/");
			}
			if (ImGui::MenuItem("Rename Level"))
			{
				m_Rename = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Create"))
		{
			std::wstring RescourceFolder = L"./Resources/Meshes/";
			if (ImGui::MenuItem("Create New Gameobject"))
			{
				scene->AddChild(m_Creator->CreateStandardGameObject(scene));

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Destroy"))
		{
			if (ImGui::MenuItem("Delete Selected GameObject"))
			{
				if (m_SelectedObject && m_SelectedObject != scene->GetChildren()[0])
				{
					if (m_SelectedObject->GetParent())
					{
						m_SelectedObject->GetParent()->RemoveChild(m_SelectedObject);
						delete m_SelectedObject;
						m_SelectedObject = nullptr;
						m_pSceneGraph->DeselectGameObject();

					}
					else if (m_SelectedObject->GetScene())
					{
						m_SelectedObject->GetScene()->RemoveChild(m_SelectedObject);
						m_SelectedObject = nullptr;
						m_pSceneGraph->DeselectGameObject();
					}
				}

			}
			ImGui::EndMenu();
		}

		EditorScene* eScene = (EditorScene*)scene;

		//pauze button
		if(eScene->GetPauseState())
		{
			if(ImGui::MenuItem("Start"))
			{
				eScene->SetPauze(false);
				eScene->SaveToFile(L"./Files/Temp/");
				m_SelectedObject = nullptr;
				m_pSceneGraph->DeselectGameObject();

				std::wstring name = eScene->GetLevelName() + L".bin";
				eScene->ReadFromFile(L"./Files/Temp/Scenes/" + name);
			}
		}
		else
		{
			if (ImGui::MenuItem("Stop"))
			{
				eScene->SetPauze(true);
				m_SelectedObject = nullptr;
				m_pSceneGraph->DeselectGameObject();

				std::wstring name = eScene->GetLevelName() +L".bin";
				eScene->ReadFromFile(L"./Files/Temp/Scenes/"+ name);
			}
		}





		std::wstring wname = eScene->GetLevelName();
		std::string name(wname.begin(), wname.end());

		
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - ImGui::CalcTextSize(name.c_str()).x/2);
		ImGui::Text(name.c_str());
		ImGui::EndMainMenuBar();
	}
}

void GUI::LoadFromFile(GameScene* scene)
{
	std::wstring scenesFolder = L"./Files/Scenes/";

	for (auto & p : std::experimental::filesystem::directory_iterator(scenesFolder))
	{
		std::wstring wname = p.path().filename();
		std::string name = std::string(wname.begin(),wname.end()-4);
		std::wstring filep = p.path();
		EditorScene* eScene = dynamic_cast<EditorScene*>(scene);

		if (ImGui::MenuItem(name.c_str()))
		{
			eScene->ClearScene();
			m_SelectedObject = nullptr;
			m_pSceneGraph->DeselectGameObject();
			eScene->ReadFromFile(filep);

		}
	}
}
void GUI::RenameLevel(GameScene* scene)
{
	ImGui::OpenPopup("Rename Scene");
	if (ImGui::BeginPopupModal("Rename Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		EditorScene* eScene = dynamic_cast<EditorScene*>(scene);
		std::wstring levlename = eScene->GetLevelName();

		std::string name = std::string(levlename.begin(), levlename.end());

		char* before = new char[100], *after = new char[100];

		strcpy_s(before, name.length() + 1, name.c_str());
		strcpy_s(after, name.length() + 1, name.c_str());
		if (ImGui::InputText("Name", after, 100, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (std::string(before) != std::string(after))
			{
				std::string afterstr = std::string(after);
				std::wstring afterwstr = std::wstring(afterstr.begin(), afterstr.end());
				eScene->SetLevelName(afterwstr);
				m_Rename = false;
			}
		}
		if(ImGui::Button("Cancel"))
		{
			m_Rename = false;
		}
		ImGui::EndPopup();

		delete[]before;
		delete[]after;
	}

}

void GUI::DrawBoundingBox()
{
	if (!m_SelectedObject)
		return;

	auto modelComps = m_SelectedObject->GetComponents<ModelComponent>();
	for(auto comp:modelComps)
	{
		auto box = comp->GetBoundingBox();
		DirectX::XMFLOAT3 corners[8];

		box.GetCorners(corners);

		DirectX::XMFLOAT4 white(1, 1, 1, 1);
		DebugRenderer::DrawLine(corners[0], corners[1],white);
		DebugRenderer::DrawLine(corners[1], corners[2],white);
		DebugRenderer::DrawLine(corners[2], corners[3],white);
		DebugRenderer::DrawLine(corners[3], corners[0],white);
													   
		DebugRenderer::DrawLine(corners[4], corners[5],white);
		DebugRenderer::DrawLine(corners[5], corners[6],white);
		DebugRenderer::DrawLine(corners[6], corners[7],white);
		DebugRenderer::DrawLine(corners[7], corners[4],white);
													   
		DebugRenderer::DrawLine(corners[0], corners[4],white);
		DebugRenderer::DrawLine(corners[1], corners[5],white);
		DebugRenderer::DrawLine(corners[2], corners[6],white);
		DebugRenderer::DrawLine(corners[3], corners[7],white);

	}

}
