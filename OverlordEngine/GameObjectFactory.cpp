#include "stdafx.h"
#include "GameObjectFactory.h"
#include "BinaryReader.h"
#include "GameObject.h"
#include "Components.h"
#include "ContentManager.h"
#include "GameScene.h"
#include "ComponentFactory.h"
GameObjectFactory::GameObjectFactory()
{
	m_CompFactory = new ComponentFactory();
}


GameObjectFactory::~GameObjectFactory()
{
	delete m_CompFactory;
}
GameObject* GameObjectFactory::CreateGameObject()
{
	return new GameObject();
}
GameObject* GameObjectFactory::CreateStandardGameObject(GameScene* scene)
{
	GameObject* standardObj = new GameObject();
	standardObj->SetName(L"NewGameObject");

	ModelComponent* pModelComp = new ModelComponent(scene->GetModels()->at(0));
	pModelComp->SetMaterial(UINT_MAX);

	standardObj->AddComponent(pModelComp);

	return standardObj;
}
GameObject* GameObjectFactory::CreateGameObjectFromFile(BinaryReader* reader, GameScene* scene,GameObject* parent )
{
	//get physxmats
	auto mats = scene->GetPhysxMaterials();

	GameObject* gObject = new GameObject();
	//add to parent if exists
	if (parent != nullptr)
	{
		parent->AddChild(gObject);
	}
	//name
	gObject->SetName(reader->ReadLongString());
	//transform
	gObject->GetTransform()->Translate(reader->Read<DirectX::XMFLOAT3>());
	//ROTATION
	gObject->GetTransform()->Rotate(reader->Read<DirectX::XMVECTOR>());
	//scale
	gObject->GetTransform()->Scale(reader->Read<DirectX::XMFLOAT3>());
	//components
	const short numOfComponents = reader->Read<unsigned char>();
	for(int i =0;i< numOfComponents;i++)
	{
		bool created = true;

		const auto compType = reader->Read<unsigned char>();
		BaseComponent* compPtr = nullptr;

		//scene components
		std::vector<BaseBehaviorComponentStruct*>* scripts = scene->GetScriptComponents();
		std::vector<TextureStruct*>* textures = scene->GetTextures();
		switch (compType)
		{
		case 0:
			created = false;
			break;
		case 1:
			compPtr =m_CompFactory->CreateFromFileModelComponent(reader,scene->GetModels());
			break;
		case 2:
			compPtr = m_CompFactory->CreateFromFileControllerComponent(reader,*mats);
			break;
		case 3:
			compPtr = m_CompFactory->CreateFromFileRigidBOdyComponent (reader);
			break;
		case 4:
			compPtr = m_CompFactory->CreateFromFileColliderComponent(reader,*mats);

			break;
		case 5:
			compPtr = m_CompFactory->CreateFromFileSpriteComponent(reader, textures);

			break;
		case 6:
			compPtr = m_CompFactory->CreateFromFileParticleEmmiterComponent(reader, textures);
			break;
		case 7:
			compPtr = m_CompFactory->CreateFromFileTerrainComponent(reader);
			break;
		case 8:
			{
			compPtr = m_CompFactory->CreateScriptComp(reader, scripts);

		}break;
		default:
			created = false;
			wchar_t typewchar = compType;
			Logger::Log(LogLevel::Warning, L"GameObjectFactory Trying to create Component with invalid type :" 
				+ gObject->GetName() + L"with component id:" + typewchar);
		}
		if(created)
		gObject->AddComponent(compPtr);
	}
	//create children
	unsigned char children = reader->Read<unsigned char>();
	for(int i=0;i<children;i++)
	{
		CreateGameObjectFromFile(reader,scene, gObject);
	}
	return gObject;
}





