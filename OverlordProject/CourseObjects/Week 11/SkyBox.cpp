
#include "stdafx.h"
#include "SkyBox.h"
#include "../../Materials/SkyBoxMaterial.h"
#include "ModelComponent.h"

SkyBox::SkyBox(const std::wstring& cubeFileName) :
	m_CubeFileName(cubeFileName)
{
}


SkyBox::~SkyBox(void)
{
}


void SkyBox::Initialize(const GameContext& gameContext)
{
	auto material = new SkyBoxMaterial();
	material->SetCubeMapTexture(m_CubeFileName);
	gameContext.pMaterialManager->AddMaterial(material, 1000);
	
	auto pModel = new ModelComponent(L"./Resources/Meshes/Box.ovm");
	pModel->SetMaterial(1000);
	AddComponent(pModel);
}