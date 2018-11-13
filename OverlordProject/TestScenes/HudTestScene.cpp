
#include "stdafx.h"
#include "HudTestScene.h"
#include "GameObject.h"
#include "Components.h"

HudTestScene::HudTestScene(void):
	GameScene(L"HudTestScene")
{
}


HudTestScene::~HudTestScene(void)
{
}

void HudTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);	

	auto obj = new GameObject();
	m_pCmpSprite = new SpriteExComponent(L"./Resources/Textures/TestSprite.jpg", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1, 1, 1, 1.0f));
	m_pCmpSprite->SetEffectType(SpriteExEffectType::RadialFill);
	obj->GetTransform()->Translate(640, 360, 0);
	//obj->GetTransform()->Scale(1.5f,1.0f,1.0f);
	obj->AddComponent(m_pCmpSprite);
	AddChild(obj);

	gameContext.pInput->AddInputAction(InputAction(1, Down, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(0, Down, VK_RIGHT));
	gameContext.pInput->AddInputAction(InputAction(2, Down, VK_SPACE));
}

void HudTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (gameContext.pInput->IsActionTriggered(2))
	{
		if (gameContext.pInput->IsActionTriggered(0))
		{
			auto currProgress = m_pCmpSprite->GetEffectProgress();
			currProgress += 0.25f * gameContext.pGameTime->GetElapsed();
			currProgress = fmod(currProgress, 1.0f);
			m_pCmpSprite->SetEffectProgress(currProgress);

			Logger::LogFormat(LogLevel::Info, L"PROGRESS: %f", currProgress);
		}
		else if (gameContext.pInput->IsActionTriggered(1))
		{
			auto currProgress = m_pCmpSprite->GetEffectProgress();
			currProgress -= 0.25f * gameContext.pGameTime->GetElapsed();
			if (currProgress < 0)
				currProgress += 1.0f;

			m_pCmpSprite->SetEffectProgress(currProgress);

			Logger::LogFormat(LogLevel::Info, L"PROGRESS: %f", currProgress);
		}
	}
	else
	{
		if (gameContext.pInput->IsActionTriggered(0))
		{
			auto currRadius = m_pCmpSprite->GetEffectParameter();
			currRadius += 0.10f * gameContext.pGameTime->GetElapsed();
			currRadius = fmod(currRadius, 1.0f);		
			m_pCmpSprite->SetEffectParameter(currRadius);

			Logger::LogFormat(LogLevel::Info, L"RADIUS: %f", currRadius);
		}
		else if (gameContext.pInput->IsActionTriggered(1))
		{
			auto currRadius = m_pCmpSprite->GetEffectParameter();
			currRadius -= 0.10f * gameContext.pGameTime->GetElapsed();
			if (currRadius < 0)
				currRadius += 1.0f;

			m_pCmpSprite->SetEffectParameter(currRadius);

			Logger::LogFormat(LogLevel::Info, L"RADIUS: %f", currRadius);
		}
	}
}

void HudTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
