
#include "stdafx.h"

#include "HardwareSkinningScene.h"
#include "GameObject.h"



#include "ContentManager.h"
#include "TextureData.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"

#include "..\..\Materials\SkinnedDiffuseMaterial.h"
#include "..\..\Materials\DiffuseMaterial.h"
#include "..\..\Materials\Deferred/DiffuseMaterial_Deferred.h"

#define ANIM_AMOUNT 5

HardwareSkinningScene::HardwareSkinningScene(void) :
	GameScene(L"HardwareSkinningScene"),
	m_pModel(nullptr)
{
}


HardwareSkinningScene::~HardwareSkinningScene(void)
{
}

void HardwareSkinningScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/PeasantGirl_Diffuse.png");

	auto diffuseMaterial_Deferred = new DiffuseMaterial_Deferred();
	diffuseMaterial_Deferred->SetDiffuseTexture(L"./Resources/Textures/PeasantGirl_Diffuse.png");

	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, diffuseMaterial_Deferred, 0);

	m_pModel = new ModelComponent(L"./Resources/Meshes/PeasantGirl.ovm");
	m_pModel->SetMaterial(0);
	auto obj = new GameObject();
	obj->AddComponent(m_pModel);
	AddChild(obj);

	m_pModel->GetAnimator()->SetAnimation(0);

	obj->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	//Input
	gameContext.pInput->AddInputAction(InputAction(0, Released, 'R'));
	gameContext.pInput->AddInputAction(InputAction(1, Released, 'P'));
	gameContext.pInput->AddInputAction(InputAction(2, Released, 'O'));

	gameContext.pInput->AddInputAction(InputAction(3, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(4, Down, VK_DOWN));

	gameContext.pInput->AddInputAction(InputAction(5, Pressed, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(6, Pressed, VK_RIGHT));
}

void HardwareSkinningScene::Update(const GameContext& gameContext)
{
	auto animator = m_pModel->GetAnimator();

	UNREFERENCED_PARAMETER(gameContext);
	if (gameContext.pInput->IsActionTriggered(0))
	{
		animator->Reset();
	}

	if (gameContext.pInput->IsActionTriggered(1))
	{
		if (animator->IsPlaying())
		{
			animator->Pause();
		}
		else
		{
			animator->Play();
		}
	}

	if (gameContext.pInput->IsActionTriggered(2))
	{
		animator->SetPlayReversed(!animator->IsReversed());
	}

	if (gameContext.pInput->IsActionTriggered(3))
	{
		auto speed = animator->GetAnimationSpeed();
		speed += 0.5f * gameContext.pGameTime->GetElapsed();
		Clamp<float>(speed, 5, 0);

		animator->SetAnimationSpeed(speed);
	}

	if (gameContext.pInput->IsActionTriggered(4))
	{
		auto speed = animator->GetAnimationSpeed();
		speed -= 0.5f * gameContext.pGameTime->GetElapsed();
		Clamp<float>(speed, 5, 0);

		animator->SetAnimationSpeed(speed);
	}

	if (gameContext.pInput->IsActionTriggered(5))
	{
		--m_AnimationIndex;
		if (m_AnimationIndex < 0)
			m_AnimationIndex = ANIM_AMOUNT-1;

		animator->SetAnimation(m_AnimationIndex);
	}

	if (gameContext.pInput->IsActionTriggered(6))
	{
		++m_AnimationIndex;
		if (m_AnimationIndex >= ANIM_AMOUNT)
			m_AnimationIndex = 0;

		animator->SetAnimation(m_AnimationIndex);
	}
}

void HardwareSkinningScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
