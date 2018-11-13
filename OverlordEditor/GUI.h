#pragma once
#include "imgui.h"
#include "GameObjectFactory.h"

class ContentBrowser;
class SceneGraph;
class ParameterWindow;
class GUI
{
public:
	GUI();
	~GUI();

	GUI(const GUI& other) = delete;
	GUI(GUI&& other) noexcept = delete;
	GUI& operator=(const GUI& other) = delete;
	GUI& operator=(GUI&& other) noexcept = delete;

	void Update(GameScene* scene);
	bool m_Demo = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
private:
	//general
	GameObject* m_SelectedObject = nullptr;
	Material* m_pSelectedMaterial = nullptr;
	PhysicsMaterialStruct* m_pSelectedPhysxMat = nullptr;


	void Picking(GameScene* scene);
	GameObject* PickGameObject(GameScene* scene);
	GameObject* PickGameObjectInChildren(GameObject* obj, DirectX::XMVECTOR directio, DirectX::XMVECTOR origin , float* foundDistanse);

	//menu bar
	void MainMenuBar(GameScene* scene);
	void LoadFromFile(GameScene* scene);
	void RenameLevel(GameScene* scene);
	void DrawBoundingBox();
	//classes
	ContentBrowser* m_PContentBrowser = nullptr;
	SceneGraph* m_pSceneGraph = nullptr;
	ParameterWindow* m_pParameterWindow = nullptr;
	GameObjectFactory* m_Creator = nullptr;

	int m_lastSet = -1;
	//values
	bool m_Rename = false;
};

