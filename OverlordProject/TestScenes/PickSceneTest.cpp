
#include "stdafx.h"
#include "PickSceneTest.h"

#include "Components.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "Prefabs.h"
#include "GameObject.h"
#include "..\CourseObjects\Week 4\Character.h"


PickSceneTest::PickSceneTest(void):
	GameScene(L"PickSceneTest"),
	m_pCharacter(nullptr)
{
}


PickSceneTest::~PickSceneTest(void)
{
}

void PickSceneTest::Initialize(const GameContext& gameContext)
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
	ground->SetTag(L"Ground");
	AddChild(ground);


	//Character
	m_pCharacter = new Character();
	m_pCharacter->SetTag(L"Character");
	AddChild(m_pCharacter);

	m_pCharacter->GetComponent<ControllerComponent>()->SetCollisionGroup(CollisionGroupFlag::Group1);
	

	// ADD SPHERE
	auto sphere1 = new SpherePrefab();
	sphere1->GetTransform()->Translate(0,5,0);

	// Sphere PhysX
	auto rigidbody = new RigidBodyComponent(true);
	sphere1->AddComponent(rigidbody);

	std::shared_ptr<physx::PxGeometry> spheregeom(new physx::PxSphereGeometry(1));
	auto collider = new ColliderComponent(spheregeom, *bouncyMaterial, physx::PxTransform::createIdentity());
	collider->EnableTrigger(true);
	sphere1->AddComponent(collider);

	sphere1->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* receiver, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(receiver);
		UNREFERENCED_PARAMETER(trigger);
		if (action == GameObject::TriggerAction::ENTER)Logger::LogInfo(L"Trigger Entered!");
	});

	sphere1->SetTag(L"Sphere");

	rigidbody->SetCollisionGroup(CollisionGroupFlag::Group3);

	AddChild(sphere1);



}

void PickSceneTest::Update(const GameContext& gameContext)
{
	if(gameContext.pInput->IsMouseButtonDown(VK_RBUTTON) && !gameContext.pInput->IsMouseButtonDown(VK_RBUTTON, true))
	{
		GameObject* pickedObject = gameContext.pCamera->Pick(gameContext, (CollisionGroupFlag)(CollisionGroupFlag::Group1));
		if (pickedObject != nullptr)
		{
			Logger::LogInfo(pickedObject->GetTag());

			//RemoveChild(pickedObject);
			//pickedObject = nullptr;
		}
	}
}

void PickSceneTest::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
