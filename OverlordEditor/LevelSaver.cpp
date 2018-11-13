#include "OverlordEditorPCH.h"
#include "LevelSaver.h"
#include "GameScene.h"
#include "EditorScene.h"
#include "Material.h"
#include "GameObject.h"
LevelSaver::LevelSaver()
{
}


LevelSaver::~LevelSaver()
{
}

void LevelSaver::SaveToFile(std::wstring basepath, GameScene* scene)
{
	SaveMaterials(basepath, scene);
	SaveModels(basepath, scene);
	SavePhysxMaterials(basepath, scene);
	SaveGameObjects(basepath, scene);
	SaveTextures(basepath, scene);

}
void LevelSaver::SaveMaterials(std::wstring basepath, GameScene* scene)
{
	BinaryWriter writer;
	auto context = scene->GetGameContext();

	//save Materials
	std::wstring matPath = basepath + L"Materials/";
	std::vector<UINT> materials = context.pMaterialManager->GetAllMaterialKeys();
	for (auto mat : materials)
	{
		if (mat == UINT_MAX)
			continue;
		std::wstring matPathfile = matPath + context.pMaterialManager->GetMaterial(mat)->GetName() + L".bin";
		writer.Open(matPathfile);

			writer.Write(mat);
			context.pMaterialManager->GetMaterial(mat)->WriteToFile(&writer);
			writer.Close();
		
	}
}

void LevelSaver::SaveModels(std::wstring basepath, GameScene* scene)
{
	std::wstring modPath = basepath + L"Models/models.bin";
	BinaryWriter writer;
	writer.Open(modPath);
	auto models = dynamic_cast<EditorScene*>(scene)->GetModels();
	UINT size = models->size();
	writer.Write(size-1);
	bool skipf = true;
	for(auto model:*models)
	{
		if(skipf)
		{
			skipf = false;
			continue;
		}
		writer.WriteString(model->modelName);
		writer.WriteLongString(model->Path);
	}
}
void LevelSaver::SavePhysxMaterials(std::wstring basepath, GameScene* scene)
{
	std::vector<PhysicsMaterialStruct*>* mats = dynamic_cast<EditorScene*>(scene)->GetPhysxMaterials();

	BinaryWriter writer;
	std::wstring matPath = basepath + L"PhysxMaterials/PHYSXMats.bin";
	writer.Open(matPath);


	UINT numPMats = (UINT)mats->size();
	writer.Write(numPMats-1);
	bool skipf = true;
	for (auto mat : *mats)
	{
		if (skipf)
		{
			skipf = false;
			continue;
		}
		writer.WriteLongString(mat->materialName);
		writer.Write(mat->material->getStaticFriction());
		writer.Write(mat->material->getDynamicFriction());
		writer.Write(mat->material->getRestitution());
	}
	writer.Close();
}
void LevelSaver::SaveGameObjects(std::wstring basepath, GameScene* scene)
{
	BinaryWriter writer;
	EditorScene* escene = dynamic_cast<EditorScene*>(scene);
	std::wstring levelpath = basepath + L"Scenes/" + escene->GetLevelName()+L".bin";
	writer.Open(levelpath);
	writer.WriteLongString(escene->GetLevelName());

	auto children = scene->GetChildren();
	UINT numChildren = children.size();
	writer.Write(numChildren - 1);
	for (UINT i = 1; i < numChildren; i++)
	{
		children[i]->SaveToFile(&writer);
	}
	writer.WriteLongString(L"Finished Saving");

}

void LevelSaver::SaveTextures(std::wstring basepath, GameScene* scene)
{
	std::wstring modPath = basepath + L"Textures/Textures.bin";
	BinaryWriter writer;
	writer.Open(modPath);
	auto textures = dynamic_cast<EditorScene*>(scene)->GetTextures();
	UINT size = textures->size();
	writer.Write(size-1);
	bool skipfirst = true;
	for (auto textures : *textures)
	{
		if(skipfirst)
		{
			skipfirst = false;
			continue;
		}
		writer.WriteString(textures->name);
		writer.WriteLongString(textures->path);
	}
}
