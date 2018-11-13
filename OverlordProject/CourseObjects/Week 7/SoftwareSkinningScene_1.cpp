
#include "stdafx.h"

#include "SoftwareSkinningScene_1.h"
#include "GameObject.h"



#include "ContentManager.h"
#include "TextureData.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "BoneObject.h"
#include "../../Materials/ColorMaterial.h"


SoftwareSkinningScene_1::SoftwareSkinningScene_1(void) :
	GameScene(L"SoftwareSkinningScene_1"),
	m_pBone0(nullptr),
	m_pBone1(nullptr),
	m_BoneRotation(0),
	m_RotationSign(1)
{
}


SoftwareSkinningScene_1::~SoftwareSkinningScene_1(void)
{
}

void SoftwareSkinningScene_1::Initialize(const GameContext& gameContext)
{
	//MATERIAL
	auto mat = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(mat, 10);

	//BONES
	auto root = new GameObject();

	m_pBone0 = new BoneObject(0, 10, 15);
	root->AddChild(m_pBone0);

	m_pBone1 = new BoneObject(1, 10, 15);
	m_pBone0->AddBone(m_pBone1);

	AddChild(root);

	//root->GetTransform()->Rotate(0, -90, 0);

	//gameContext.pCamera->GetTransform()->Translate(10, 2, 10);
}

void SoftwareSkinningScene_1::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();

	if (m_RotationSign<0 && m_BoneRotation <= -45)m_RotationSign = 1;
	else if (m_RotationSign>0 && m_BoneRotation >= 45)m_RotationSign = -1;

	m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
	m_pBone1->GetTransform()->Rotate(0,0,-m_BoneRotation * 2);
}

void SoftwareSkinningScene_1::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
