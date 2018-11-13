
#include "stdafx.h"

#include "UberScene.h"
#include "GameObject.h"

#include "Components.h"
#include "PhysxProxy.h"
#include "PhysxManager.h"
#include "ContentManager.h"
#include "../../Materials/UberMaterial.h"
#include "ModelComponent.h"
#include "SoundManager.h"

#define FPS_COUNTER 1

UberScene::UberScene(void):
	GameScene(L"UberScene"),
	m_FpsInterval(FPS_COUNTER)
{
}


UberScene::~UberScene(void)
{
}

void UberScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0,0,1)))));
	AddChild(ground);

	auto fmodSystem = SoundManager::GetInstance()->GetSystem();
	FMOD::Sound* pSound = nullptr;
	auto fmodResult = fmodSystem->createStream("Resources/Sounds/wave.mp3", FMOD_DEFAULT, nullptr, &pSound);
	SoundManager::ErrorCheck(fmodResult);

	pSound->setMode(FMOD_LOOP_NORMAL);

	fmodResult = fmodSystem->playSound(pSound, nullptr, false, nullptr);
	SoundManager::ErrorCheck(fmodResult);

	//Material Test
	auto myMaterial = new UberMaterial();
	myMaterial->EnableSpecularPhong(true);
	myMaterial->EnableEnvironmentMapping(true);
	myMaterial->SetEnvironmentCube(L"./Resources/Textures/Sunol_Cubemap.dds");
	myMaterial->SetReflectionStrength(0.5f);
	myMaterial->EnableFresnelFaloff(true);
	myMaterial->SetDiffuseColor(static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Red));
	myMaterial->SetFresnelMultiplier(5.5f);
	myMaterial->SetFresnelHardness(0.5f);
	myMaterial->SetFresnelPower(5.f);
	//myMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	//myMaterial->EnableDiffuseTexture(true);
	gameContext.pMaterialManager->AddMaterial(myMaterial, 50);
	
	auto model = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	model->SetMaterial(50);

	auto teapot = new GameObject();
	teapot->AddComponent(model);
	AddChild(teapot);
	teapot->GetTransform()->Translate(0,0,20);
}

void UberScene::Update(const GameContext& gameContext)
{

	m_FpsInterval += gameContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	}
}

void UberScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
