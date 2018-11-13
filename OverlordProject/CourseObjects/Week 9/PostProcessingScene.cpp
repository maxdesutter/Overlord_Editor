
#include "stdafx.h"

#include "PostProcessingScene.h"
#include "GameObject.h"



#include "ContentManager.h"
#include "TextureData.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"
#include "SpriteFont.h"
#include "TextRenderer.h"

#include "..\..\Materials\SkinnedDiffuseMaterial.h"
#include "PostBlur.h"
#include "PostGrayscale.h"

PostProcessingScene::PostProcessingScene(void) :
	GameScene(L"PostProcessingScene"),
	m_pModel(nullptr),
	m_pFont(nullptr)
{
}


PostProcessingScene::~PostProcessingScene(void)
{
}

void PostProcessingScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 0);

	m_pModel = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	m_pModel->SetMaterial(0);
	auto obj = new GameObject();
	obj->AddComponent(m_pModel);
	AddChild(obj);

	obj->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	//Input
	gameContext.pInput->AddInputAction(InputAction(0, Released, 'R'));
	gameContext.pInput->AddInputAction(InputAction(1, Released, 'P'));
	gameContext.pInput->AddInputAction(InputAction(2, Released, 'O'));

	gameContext.pInput->AddInputAction(InputAction(3, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(4, Down, VK_DOWN));

	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");

	AddPostProcessingEffect(new PostBlur());
	AddPostProcessingEffect(new PostGrayscale());
}

void PostProcessingScene::Update(const GameContext& gameContext)
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
}

void PostProcessingScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	TextRenderer::GetInstance()->DrawText(m_pFont, L"Post Processing", DirectX::XMFLOAT2(10, 10), (DirectX::XMFLOAT4) DirectX::Colors::Red);
}
