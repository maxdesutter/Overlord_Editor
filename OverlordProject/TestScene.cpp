
#include "stdafx.h"

#include "TestScene.h"
#include "GameObject.h"



#include "Prefabs.h"
#include "Components.h"
#include "PhysxProxy.h"
#include "PhysxManager.h"
#include "ContentManager.h"
#include "MeshFilter.h"
#include "Materials\ColorMaterial.h"
#include "Materials\UberMaterial.h"
#include "ModelComponent.h"
#include "CourseObjects\Week 3\Pong\Peddle.h"
#include "CourseObjects\Week 4\Character.h"

#define FPS_COUNTER 1

TestScene::TestScene(void):
	GameScene(L"TestScene"),
	m_pTestObj1(nullptr),
	m_pTestObj2(nullptr),
	m_FpsInterval(FPS_COUNTER)
{
}


TestScene::~TestScene(void)
{
}

void TestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	//GetPhysxProxy()->EnablePhysXFrameStepping(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial,physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0,0,1)))));
	AddChild(ground);

	//auto level = new GameObject();

	gameContext.pMaterialManager->AddMaterial(new ColorMaterial(), 0);

	//SPHERE 1
	auto sphere = new SpherePrefab();
	sphere->GetTransform()->Translate(0, 30, 0);
	auto rigidbody = new RigidBodyComponent();

	rigidbody->SetCollisionGroup(CollisionGroupFlag::Group0); //SET GROUP
	rigidbody->SetCollisionIgnoreGroups(CollisionGroupFlag::Group1); //IGNORE GROUP
	
	sphere->AddComponent(rigidbody);
	std::shared_ptr<physx::PxGeometry> spheregeom(new physx::PxSphereGeometry(1));
	sphere->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(sphere);

	//SPHERE 2
	sphere = new SpherePrefab();
	sphere->GetTransform()->Translate(0, 20, 0);
	rigidbody = new RigidBodyComponent();

	rigidbody->SetCollisionGroup(CollisionGroupFlag::Group1); //SET GROUP

	sphere->AddComponent(rigidbody);
	std::shared_ptr<physx::PxGeometry> spheregeom1(new physx::PxSphereGeometry(1));
	sphere->AddComponent(new ColliderComponent(spheregeom1, *bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(sphere);

	//SPHERE 3
	sphere = new SpherePrefab();
	sphere->GetTransform()->Translate(0, 10, 0);
	rigidbody = new RigidBodyComponent();

	rigidbody->SetCollisionGroup(CollisionGroupFlag::Group1); //SET GROUP

	sphere->AddComponent(rigidbody);
	std::shared_ptr<physx::PxGeometry> spheregeom2(new physx::PxSphereGeometry(1));
	sphere->AddComponent(new ColliderComponent(spheregeom2, *bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(sphere);
}

void TestScene::Update(const GameContext& gameContext)
{

	m_FpsInterval += gameContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	}

}

void TestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
