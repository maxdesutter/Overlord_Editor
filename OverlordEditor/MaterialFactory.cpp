#include "OverlordEditorPCH.h"
#include "MaterialFactory.h"
#include "BinaryReader.h"

#include "ColorMaterial.h"
#include "DiffuseMaterial.h"
#include "SkinnedDiffuseMaterial.h"
#include "SkyBoxMaterial.h"
#include "SpikeyMaterial.h"
#include "UberMaterial.h"
#include "EditorScene.h"

MaterialFactory::MaterialFactory()
{
}
MaterialFactory::~MaterialFactory()
{
}
Material* MaterialFactory::CreateMaterialFromFile(BinaryReader* reader,  GameScene* scene)
{
	std::wstring materialName = reader->ReadLongString();

	int materialType = reader->Read<unsigned char>();
	Material* matPtr = nullptr;
	switch (materialType)
	{
	case 0:
		matPtr = CreateColorMatFromFile(reader);
		break;
	case 1:
		matPtr = CreateDiffusematerialFromFile(reader,scene);
		break;
	case 2:
		matPtr = CreateSkinneddiffuseFromFile(reader,scene);
		break;
	case 3:
		matPtr = CreateSkyboxmatFromFile(reader,scene);
		break;
	case 4:
		matPtr = CreateSpikymatFromFile(reader,scene);
		break;
	case 5:
		matPtr = CreateUbermaterialFromFile(reader,scene);
		break;
	default:
		Logger::Log(LogLevel::Error, L"MaterialFactory Trying to create material with invalid type :" + materialName);
		return nullptr;
	}
	matPtr->SetName(materialName);
	return matPtr;
}
Material* MaterialFactory::CreateColorMatFromFile(BinaryReader* reader)
{
	auto col =	 new ColorMaterial();
	col->SetColor(reader->Read<DirectX::XMFLOAT4>());
	return col;
}
Material* MaterialFactory::CreateDiffusematerialFromFile(BinaryReader* reader, GameScene* scene)
{
	DiffuseMaterial* diffMat = new DiffuseMaterial();
	diffMat->SetDiffuseTexture(reader->ReadLongString());
	return diffMat;
}
Material* MaterialFactory::CreateSkinneddiffuseFromFile(BinaryReader* reader, GameScene* scene)
{
	SkinnedDiffuseMaterial* skinMat = new SkinnedDiffuseMaterial();

	std::wstring wname = reader->ReadString();
	std::string name(wname.begin(), wname.end());

	std::vector<TextureStruct*>* textures = ((EditorScene*)scene)->GetTextures();

	for(auto tex:*textures)
	{
		if (tex->name==name)
		{
			skinMat->SetDiffuseTextureStruct(tex);
			return skinMat;
		}
	}
	skinMat->SetDiffuseTextureStruct(textures->at(0));
	return skinMat;

}
Material* MaterialFactory::CreateSkyboxmatFromFile(BinaryReader* reader, GameScene* scne)
{
	SkyBoxMaterial* skyMat = new SkyBoxMaterial();
	skyMat->SetCubeMapTexture(reader->ReadLongString());
	return skyMat;
}
Material* MaterialFactory::CreateSpikymatFromFile(BinaryReader* reader, GameScene* scne)
{
	SpikeyMaterial* spikyMatt = new SpikeyMaterial();
	spikyMatt->SetDiffuseTexture(reader->ReadLongString());
	return spikyMatt;
}
Material* MaterialFactory::CreateUbermaterialFromFile(BinaryReader* reader, GameScene* scne)
{
	UberMaterial* ubMat = new UberMaterial();
	std::string name;
	auto textures = dynamic_cast<EditorScene*>(scne)->GetTextures();
	//lightdirection
	ubMat->SetLightDirection(reader->Read<DirectX::XMFLOAT3>());

	//diffuse
	ubMat->EnableDiffuseTexture(reader->Read<bool>());


	name = reader->ReadRegString();
	if(name!="")
	{
		for(auto tex: *textures)
		{
			if(tex->name==name)
			{
				ubMat->SetDiffuseTextureStruct(tex);
			}
		}
	}
	ubMat->SetDiffuseColor(reader->Read<DirectX::XMFLOAT4>());

	//Specular
	ubMat->SetSpecularColor(reader->Read<DirectX::XMFLOAT4>());
	ubMat->EnableSpecularLevelTexture(reader->Read<bool>());
	name = reader->ReadRegString();
	if (name != "")
	{
		for (auto tex : *textures)
		{
			if (tex->name == name)
			{
				ubMat->SetSpecularLevelTextureStruct(tex);
			}
		}
	}
	ubMat->SetShininess(reader->Read<int>());

	//ambient
	ubMat->SetAmbientColor(reader->Read<DirectX::XMFLOAT4>());
	ubMat->SetAmbientIntensity(reader->Read<float>());

	//normal
	ubMat->FlipNormalGreenCHannel(reader->Read<bool>());
	ubMat->EnableNormalMapping(reader->Read<bool>());
	name = reader->ReadRegString();
	if (name != "")
	{
		for (auto tex : *textures)
		{
			if (tex->name == name)
			{

				ubMat->SetNormalMapTextureStruct(tex);
			}
		}
	}

	//enviroment
	ubMat->EnableEnvironmentMapping(reader->Read<bool>());
	name = reader->ReadRegString();
	if (name != "")
	{
		for (auto tex : *textures)
		{
			if (tex->name == name)
			{

				ubMat->SetEnvironmentCubeStruct(tex);
			}
		}
	}
	ubMat->SetReflectionStrength(reader->Read<float>());
	ubMat->SetRefractionStrength(reader->Read<float>());
	ubMat->SetRefractionIndex(reader->Read<float>());

	//opacity
	ubMat->SetOpacity(reader->Read<float>());
	ubMat->EnableOpacityMap(reader->Read<bool>());
	name = reader->ReadRegString();
	if (name != "")
	{
		for (auto tex : *textures)
		{
			if (tex->name == name)
			{

				ubMat->SetOpacityTextureStruct(tex);
			}
		}
	}

	//spec blinn phong
	ubMat->EnableSpecularBlinn(reader->Read<bool>());
	ubMat->EnableSpecularPhong(reader->Read<bool>());

	//fresnel
	ubMat->EnableFresnelFaloff(reader->Read<bool>());
	ubMat->SetFresnelColor(reader->Read<DirectX::XMFLOAT4>());
	ubMat->SetFresnelPower(reader->Read<float>());
	ubMat->SetFresnelMultiplier(reader->Read<float>());
	ubMat->SetFresnelHardness(reader->Read<float>());




	return ubMat;
}

