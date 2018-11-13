
#include "stdafx.h"
#include "TutorialScene.h"

#include "Components.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "Prefabs.h"
#include "GameObject.h"

TutorialScene::TutorialScene(void):
	GameScene(L"TutorialScene"),
	m_pSphere(nullptr)
{
}


TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	
	m_PxMat = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*m_PxMat, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0,0,1)))));
	AddChild(ground);

	// ADD SPHERE
	m_pSphere = new SpherePrefab();
	m_pSphere->GetTransform()->Translate(0,5,0);

	// Sphere PhysX
	auto rigidbody = new RigidBodyComponent();
	
	m_pSphere->AddComponent(rigidbody);

	std::shared_ptr<physx::PxGeometry> spheregeom(new physx::PxSphereGeometry(1));
	m_pSphere->AddComponent(new ColliderComponent(spheregeom,*m_PxMat, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0,0,1)))));

	AddChild(m_pSphere);

	auto inputAction = InputAction(0, InputTriggerState::Down, 'M');
	gameContext.pInput->AddInputAction(inputAction);
	std::wstring* tex = new std::wstring(L"banaan");
	m_PxMat->userData = tex;
}

void TutorialScene::Update(const GameContext& gameContext)
{
	if(gameContext.pInput->IsActionTriggered(0))
	{
		m_pSphere->GetTransform()->Translate(0,10,0);
	}
	
	//UNREFERENCED_PARAMETER(data);
} 

void TutorialScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
