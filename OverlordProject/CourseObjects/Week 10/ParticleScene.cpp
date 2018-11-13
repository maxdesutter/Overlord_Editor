
#include "stdafx.h"

#include "ParticleScene.h"
#include "GameObject.h"

#include "ModelAnimator.h"
#include "TransformComponent.h"
#include "ParticleEmitterComponent.h"
#include "ModelComponent.h"
#include "../../Materials/SkinnedDiffuseMaterial.h"

ParticleScene::ParticleScene(void) :
	GameScene(L"ParticleScene"),
	m_pParticleEmitter(nullptr),
	m_pModel(nullptr)
{
}


ParticleScene::~ParticleScene(void)
{
}

void ParticleScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//gameContext.pInput->ForceMouseToCenter(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto obj = new GameObject();

	m_pParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/Smoke.png", 60);
	m_pParticleEmitter->SetVelocity(DirectX::XMFLOAT3(0, 6.0f, 0));
	m_pParticleEmitter->SetMinSize(1.0f);
	m_pParticleEmitter->SetMaxSize(2.0f);
	m_pParticleEmitter->SetMinEnergy(1.0f);
	m_pParticleEmitter->SetMaxEnergy(2.0f);
	m_pParticleEmitter->SetMinSizeGrow(3.5f);
	m_pParticleEmitter->SetMaxSizeGrow(5.5f);
	m_pParticleEmitter->SetMinEmitterRange(0.2f);
	m_pParticleEmitter->SetMaxEmitterRange(0.5f);
	m_pParticleEmitter->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 0.6f));
	obj->AddComponent(m_pParticleEmitter);
	AddChild(obj);

	auto mat = new SkinnedDiffuseMaterial();
	mat->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	gameContext.pMaterialManager->AddMaterial(mat, 0);

	auto obj2 = new GameObject();
	m_pModel = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	m_pModel->SetMaterial(0);
	obj2->AddComponent(m_pModel);
	AddChild(obj2);
	m_pModel->GetAnimator()->Play();
	obj2->GetTransform()->Scale(.1f, .1f, .1f);
	//obj2->GetTransform()->Translate(0.f,0.f,10.f);

	gameContext.pInput->AddInputAction(InputAction(0, Down, VK_RIGHT));
	gameContext.pInput->AddInputAction(InputAction(1, Down, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(2, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(3, Down, VK_DOWN));
}

void ParticleScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto initPos = m_pParticleEmitter->GetTransform()->GetPosition();
	auto moveSpeed = 10.0f * gameContext.pGameTime->GetElapsed();
	initPos.x += moveSpeed * sin(gameContext.pGameTime->GetTotal());


	if (gameContext.pInput->IsActionTriggered(0))
	{
		initPos.x += moveSpeed;
	}
	if (gameContext.pInput->IsActionTriggered(1))
	{
		initPos.x -= moveSpeed;
	}
	if (gameContext.pInput->IsActionTriggered(2))
	{
		initPos.z += moveSpeed;
	}
	if (gameContext.pInput->IsActionTriggered(3))
	{
		initPos.z -= moveSpeed;
	}
	m_pParticleEmitter->GetTransform()->Translate(initPos);
}

void ParticleScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
