#include "OverlordEditorPCH.h"
#include "ContentBrowser.h"
#include "imgui.h"
#include "GameScene.h"
#include "Material.h"
#include "EditorScene.h"
#include <commdlg.h>
#include "FileAdder.h"
#include "TextureData.h"
#include "ContentManager.h"
#include "SkinnedDiffuseMaterial.h"
#include "SkyBoxMaterial.h"
#include "SpikeyMaterial.h"
#include "UberMaterial.h"
#include "ColorMaterial.h"

ContentBrowser::ContentBrowser()
{
}


ContentBrowser::~ContentBrowser()
{

}
void ContentBrowser::Update( GameScene* scene)
{
	const GameContext& gameContext = scene->GetGameContext();



	ImGui::SetNextWindowPos({ 300,700 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 1200,200 }, ImGuiCond_Once);
	ImGuiWindowFlags flg;
	flg = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Content Browser", 0, flg);
	ContentMenu(scene);
	
	static int ContentSelected = -1;

	ImGui::BeginChild("contentbrowesermenu", ImVec2(120, 0), true);
	if (ImGui::Selectable("Materials", ContentSelected == 0, 0, ImVec2(100, 15)))
	{
		ContentSelected = 0;
	}
	if (ImGui::Selectable("Models", ContentSelected == 1, 0, ImVec2(100, 15)))
	{
		ContentSelected = 1;
	}
	if (ImGui::Selectable("Textures", ContentSelected == 2, 0, ImVec2(100, 15)))
	{
		ContentSelected = 2;
	}
	if (ImGui::Selectable("PhysxMaterials", ContentSelected == 3, 0, ImVec2(100, 15)))
	{
		ContentSelected = 3;
	}
	if (ImGui::Selectable("Behaviors", ContentSelected == 4, 0, ImVec2(100, 15)))
	{
		ContentSelected = 4;
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("Contentbrowesercontent");


	switch (ContentSelected)
	{
		//materials selected
		case 0:
		{
			ShowMaterials(gameContext);
		}
		break;
		//models selected
		case 1:
		{
			ShowModels(scene);
		}
		break;
		case 2:
		{
			ShowTextures(scene);
		}
		break;
		case 3:
		{
			ShowPhyscMaterials(scene);
		}
		break;
		case 4:
		{
			ShowBehaviorComponents(scene);
		}
		break;
		default:
		{}
	}
	ImGui::EndChild();

	ImGui::End();

}
void ContentBrowser::ContentMenu(GameScene* scene)
{
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create Item"))
		{
			if (ImGui::BeginMenu("Create Material"))
			{
				if (ImGui::MenuItem("Create Collor Material"))
				{
					ColorMaterial* mat = new ColorMaterial();
					scene->GetGameContext().pMaterialManager->AddMaterial(mat, scene->GetGameContext().pMaterialManager->GetAllMaterialKeys().size());
				}
				if(ImGui::MenuItem("Create Diffuse Material"))
				{
					SkinnedDiffuseMaterial* mat = new SkinnedDiffuseMaterial();
					mat->SetDiffuseTextureStruct(dynamic_cast<EditorScene*>(scene)->GetTextures()->at(0));
					scene->GetGameContext().pMaterialManager->AddMaterial(mat,scene->GetGameContext().pMaterialManager->GetAllMaterialKeys().size());
				}
				if (ImGui::MenuItem("Create SkyBox Material"))
				{
					SkyBoxMaterial* mat = new SkyBoxMaterial();
					mat->SetTextureStruct(dynamic_cast<EditorScene*>(scene)->GetTextures()->at(0));

					scene->GetGameContext().pMaterialManager->AddMaterial(mat, scene->GetGameContext().pMaterialManager->GetAllMaterialKeys().size());
				}
				if (ImGui::MenuItem("Create Spikey Material"))
				{
					SpikeyMaterial* mat = new SpikeyMaterial();
					mat->SetTextureStruct(dynamic_cast<EditorScene*>(scene)->GetTextures()->at(0));

					scene->GetGameContext().pMaterialManager->AddMaterial(mat, scene->GetGameContext().pMaterialManager->GetAllMaterialKeys().size());

				}
				if (ImGui::MenuItem("Create Uber Material"))
				{
					UberMaterial* mat = new UberMaterial();
					
					scene->GetGameContext().pMaterialManager->AddMaterial(mat, scene->GetGameContext().pMaterialManager->GetAllMaterialKeys().size());

				}
				ImGui::EndMenu();
			}
			
			if (ImGui::MenuItem("Add Model to contentbrowser"))
			{
				FileAdder adder;
				ModelReflectionStruct* str = adder.GetOvmFile();
				if(str)
					dynamic_cast<EditorScene*>(scene)->GetModels()->push_back(str );
				 
			}
			if (ImGui::MenuItem("Add Texture to contentbrowser"))
			{
				FileAdder adder;
				TextureStruct* str = adder.GetTexture();
				if(str)
					dynamic_cast<EditorScene*>(scene)->GetTextures()->push_back(str);
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

	}
}
void ContentBrowser::ShowMaterials(const GameContext& gameContext)
{

	auto matManager = gameContext.pMaterialManager;
	auto matKeys = matManager->GetAllMaterialKeys();
	float windowWidth = ImGui::GetWindowWidth();
	float windowPosX = ImGui::GetWindowPos().x;

	for (UINT matKey : matKeys)
	{
		std::wstring namew = matManager->GetMaterial(matKey)->GetName();
		std::string name(namew.begin(), namew.end());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
		

			if (ImGui::Button(name.c_str(), ImVec2(100, 60)))
			{
				m_SelectedMaterial = matManager->GetMaterial(matKey);
				m_selectedPhysxMat = nullptr;
			}
		

		ImGui::PopStyleColor();
		//wrapping
		float lastButtonMaxXValue = ImGui::GetItemRectMax().x;
		if (lastButtonMaxXValue < windowPosX + windowWidth - 100)
			ImGui::SameLine();

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("DND_Material", &matKey, sizeof(UINT));        // Set payload to carry the index of our item (could be anything)
			ImGui::Text("Place %s material", name.c_str());
			ImGui::EndDragDropSource();
		}

	}
}
void ContentBrowser::ShowModels(GameScene* scene)
{
	std::vector<ModelReflectionStruct*>*modelStructVectors;
	modelStructVectors = ((EditorScene*)scene)->GetModels();
	float windowWidth = ImGui::GetWindowWidth();
	float windowPosX = ImGui::GetWindowPos().x;
	for (size_t i = 0; i < modelStructVectors->size(); i++)
	{

		ImGui::Button(modelStructVectors->at(i)->modelName.c_str(), ImVec2(100, 60));
		float lastButtonMaxXValue = ImGui::GetItemRectMax().x;
		if (lastButtonMaxXValue < windowPosX + windowWidth - 100)
			ImGui::SameLine();

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("DND_Model", &modelStructVectors->at(i), sizeof(ModelReflectionStruct*));        // Set payload to carry the index of our item (could be anything)

			ImGui::Text("Place %s Model", modelStructVectors->at(i)->modelName.c_str());

			ImGui::EndDragDropSource();
		}
	}
}
void ContentBrowser::ShowBehaviorComponents(GameScene* scene)
{
	EditorScene* eScene = dynamic_cast<EditorScene*>(scene);
	float windowWidth = ImGui::GetWindowWidth();
	float windowPosX = ImGui::GetWindowPos().x;
	for (size_t i = 0; i < eScene->GetScriptComponents()->size(); i++)
	{

		ImGui::Button(eScene->GetScriptComponents()->at(i)->name.c_str(), ImVec2(100, 60));
		float lastButtonMaxXValue = ImGui::GetItemRectMax().x;
		if (lastButtonMaxXValue < windowPosX + windowWidth - 100)
			ImGui::SameLine();
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("DND_Behavior_Component", &(eScene->GetScriptComponents()->at(i)), sizeof(BaseBehaviorComponentStruct*));

			ImGui::Text("Place %s Script", eScene->GetScriptComponents()->at(i)->name.c_str());

			ImGui::EndDragDropSource();
		}

	}
}
void ContentBrowser::ShowPhyscMaterials(GameScene* scene)
{
	std::vector<PhysicsMaterialStruct*>*matStructVectors;
	matStructVectors = ((EditorScene*)scene)->GetPhysxMaterials();
	float windowWidth = ImGui::GetWindowWidth();
	float windowPosX = ImGui::GetWindowPos().x;
	for (size_t i = 0; i < matStructVectors->size(); i++)
	{
		std::string name = std::string(matStructVectors->at(i)->materialName.begin(), matStructVectors->at(i)->materialName.end());
		if(ImGui::Button(name.c_str(), ImVec2(100, 60)))
		{
			m_selectedPhysxMat = matStructVectors->at(i);
			m_SelectedMaterial = nullptr;
		}
		float lastButtonMaxXValue = ImGui::GetItemRectMax().x;
		if (lastButtonMaxXValue < windowPosX + windowWidth - 100)
			ImGui::SameLine();

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("DND_PhysxMaterial", &matStructVectors->at(i), sizeof(PhysicsMaterialStruct*));        // Set payload to carry the index of our item (could be anything)

			ImGui::Text("Place %s PMaterial", name.c_str());

			ImGui::EndDragDropSource();
		}
	}
}
void ContentBrowser::ShowTextures(GameScene* scene)
{
	std::vector<TextureStruct*>*matStructVectors;
	matStructVectors = ((EditorScene*)scene)->GetTextures();
	
	auto initialPosition = ImGui::GetCursorPos();

	for (int i = 0; i < (int)matStructVectors->size(); i++)
	{
		auto  m_pDiffuseTexture = ContentManager::Load<TextureData>(matStructVectors->at(i)->path);

		ImTextureID texture = m_pDiffuseTexture->GetShaderResourceView();

		std::string unqieId = "ContentTexture:" + std::to_string(i);

		const char*  uniqueId = unqieId.c_str();
		ImGui::PushID(uniqueId);

		//button position logic
		ImGui::SetCursorPos({ initialPosition.x + i % 8 * 120, initialPosition.y + 85 * int(i / 8) });
		ImGui::ImageButton(texture, ImVec2(100, 60));

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("DND_Textures", &matStructVectors->at(i), sizeof(TextureStruct*));        // Set payload to carry the index of our item (could be anything)

			ImGui::Text("Place %s Texture", matStructVectors->at(i)->name.c_str());

			ImGui::EndDragDropSource();
		}
		ImGui::PopID();


		//Text position logic
		ImGui::SetCursorPos({ initialPosition.x + i % 8 * 120, initialPosition.y + 65 + 85 * int(i / 8) });

		ImGui::Text(matStructVectors->at(i)->name.c_str());

	}
}
