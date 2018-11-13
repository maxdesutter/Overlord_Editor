#include "OverlordEditorPCH.h"
#include "FileReader.h"
#include <filesystem>

#include "Material.h"
#include "MaterialFactory.h"
#include "BinaryReader.h"

#include "GameScene.h"
#include "EditorScene.h"
#include "PhysicsFactory.h"
#include "PhysxManager.h"
#include "GameObjectFactory.h"

FileReader::FileReader()
{
	m_MatFactory = new MaterialFactory();
}


FileReader::~FileReader()
{
	delete m_MatFactory;
}

void FileReader::ReadLevel(std::wstring basepast, GameScene* scene)
{
	//clear the scene of existing objects
	auto escene = dynamic_cast<EditorScene*>(scene);
	escene->ClearScene();

	BinaryReader reader;
	reader.Open(basepast);
	//check if file is valid
	std::wstring checksentenct = L"Finished Saving";
	if (reader.GetFinalWord(checksentenct.length()) != checksentenct)
	{
		Logger::LogWarning(L"Sorry that savefile is corupted");

		return;
	}
	reader.SetBufferPosition(0);



	escene->SetLevelName(reader.ReadLongString());

	GameObjectFactory gameObjectFact;

	int numObs = reader.Read<int>();
	for (int i = 0; i < numObs; i++)
	{
		scene->AddChild(gameObjectFact.CreateGameObjectFromFile(&reader, escene));
	}


}

void FileReader::ReadMaterials(std::wstring basepast, GameScene* scene)
{
	std::wstring path = basepast + L"/Materials";
	BinaryReader reader;
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
	{
		std::wstring filep = p.path();
		reader.Open(filep);
		if(!reader.Exists())
		{
			reader.Close();

			continue;
		}
		UINT location = reader.Read<UINT>();
		Material* mat= m_MatFactory->CreateMaterialFromFile(&reader,scene);
		reader.Close();
		scene->GetGameContext().pMaterialManager->AddMaterial(mat, location);
	}
}
void FileReader::ReadModel(std::wstring basepast, GameScene* scene)
{
	std::wstring path = basepast + L"/Models/models.bin";
	BinaryReader reader;
	reader.Open(path);
	if (!reader.Exists())
	{
		reader.Close();

		return;
	}
	UINT num = reader.Read<UINT>();
	for(UINT i=0;i<num;i++)
	{
		std::wstring wname = reader.ReadString();
		std::wstring modelPath = reader.ReadLongString();
		std::string name = std::string(wname.begin(), wname.end());

		

		dynamic_cast<EditorScene*>(scene)->GetModels()->push_back( new ModelReflectionStruct(name, modelPath));
	}
	reader.Close();
}
void FileReader::ReadPhysxMaterials(std::wstring basepath, GameScene* scene)
{
	std::wstring matPath = basepath + L"/PhysxMaterials/PHYSXMats.bin";
	BinaryReader reader;
	reader.Open(matPath);
	if (reader.Exists())
	{
		UINT num = reader.Read<UINT>();
		auto mats = dynamic_cast<EditorScene*>(scene)->GetPhysxMaterials();
		auto physX = PhysxManager::GetInstance()->GetPhysics();


		PhysicsFactory physFact;
		for (UINT i = 0; i < num; i++)
		{
			mats->push_back(physFact.CreatePhysicsMaterialFromFile(&reader, physX));
		}
	}
	reader.Close();
	
}

void FileReader::ReadTextures(std::wstring basepast, GameScene* scene)
{
	std::wstring path = basepast + L"/Textures/Textures.bin";
	BinaryReader reader;
	reader.Open(path);
	if(!reader.Exists())
	{
		reader.Close();
		return;
	}
	UINT num = reader.Read<UINT>();
	for (UINT i = 0; i < num; i++)
	{
		std::wstring wname = reader.ReadString();
		std::wstring texturePath = reader.ReadLongString();
		std::string name = std::string(wname.begin(), wname.end());



		dynamic_cast<EditorScene*>(scene)->GetTextures()->push_back(new TextureStruct( texturePath,name));
	}
	reader.Close();
}

