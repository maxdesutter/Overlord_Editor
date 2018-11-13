#include "OverlordEditorPCH.h"

#include "EditorScene.h"

#include "PhysxManager.h"
#include "PhysxProxy.h"

#include "GameObject.h"

#include "GameObjectFactory.h"



#include "GUI.h"
#include "ComponentFactory.h"

#include "LevelSaver.h"
#include "FileReader.h"
#include "MovementScript.h"
#include "ColorMaterial.h"

EditorScene::EditorScene():GameScene(L"EditorScene")
{
	m_ScriptComponents.push_back(new BehaviorComponentStruct<MovementScript>());
	std::string name = typeid(MovementScript).name();

	int i = 0;
}
EditorScene::~EditorScene()
{
	delete m_Gui;
	delete m_ComponentCreater;

	delete m_Saver;
	delete m_Reader;


	for(auto pmats:m_VecPhysicsMaterialsPtr)
	{
		delete pmats;
	}
	for (auto models : m_Models)
	{
		delete models;
	}
	for (auto textures : m_Textures)
	{
		delete textures;
	}
	for (auto scr : m_ScriptComponents)
	{
		delete scr;
	}
}

void EditorScene::ClearScene()
{
	for(size_t i =1;i<GetChildren().size();)
	{
		RemoveChild(GetChildren()[i]);
	}
}

void EditorScene::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	m_LevelName = L"testLevel";

	//add default settings
	//material
	m_pDefMat = new ColorMaterial();
	m_pDefMat->SetName(L"DefaultMaterial");
	gameContext.pMaterialManager->AddMaterial(m_pDefMat,UINT_MAX);
	//texture
	m_Textures.push_back(new TextureStruct(L"Resources/Textures/TestSprite.jpg", "Default Texture"));
	//model
	m_Models.push_back(new ModelReflectionStruct("DefaultModel", L"Resources/Meshes/StandardGameObject.ovm"));
	//px mat
	m_VecPhysicsMaterialsPtr.push_back(new PhysicsMaterialStruct(physX->createMaterial(0, 0, 0), L"DefaultPxMaterial"));

	m_Saver = new LevelSaver();
	m_Reader = new FileReader();

	m_Reader->ReadTextures(L"./Files", this);
	m_Reader->ReadMaterials(L"./Files", this);
	m_Reader->ReadModel(L"./Files", this);
	m_Reader->ReadPhysxMaterials(L"./Files", this);

	
	m_Gui = new GUI();

	m_ComponentCreater = new ComponentFactory();
}
void EditorScene::Update(const GameContext& )
{
	m_Gui->Update(this);
	static bool testb = true;
	if (testb)
	{
		SetPauze(true);
		testb = false;
	}


}
void EditorScene::Draw(const GameContext& )
{


}

void EditorScene::ReadFromFile(std::wstring filePath)
{
	m_Reader->ReadLevel(filePath, this);

}
void EditorScene::SaveToFile(std::wstring path)
{
	m_Saver->SaveToFile(path, this);
	}

