
#include "stdafx.h"

#include "CharacterTestScene.h"
#include "GameObject.h"


#include "Prefabs.h"
#include "Components.h"
#include "PhysxProxy.h"
#include "PhysxManager.h"
#include "ContentManager.h"
#include "MeshFilter.h"
#include "ModelComponent.h"
#include "PhysxHelper.h"

#include "..\Materials\ColorMaterial.h"
#include "..\Materials\DiffuseMaterial.h"
#include "..\CourseObjects\Week 4\Character.h"


CharacterTestScene::CharacterTestScene(void) :
GameScene(L"CharacterTestScene"),
m_pCharacter(nullptr)
{
}


CharacterTestScene::~CharacterTestScene(void)
{
}

void CharacterTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	//GetPhysxProxy()->EnablePhysXFrameStepping(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(1.0f, 1.0f, 0.1f);
	/*auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));*/
	//AddChild(ground);

	//Character
	m_pCharacter = new Character();
	AddChild(m_pCharacter);

	auto obj = new GameObject();

	std::shared_ptr<physx::PxGeometry> geom2(new physx::PxBoxGeometry(2,2,2));
	auto collider = new ColliderComponent(geom2, *bouncyMaterial, physx::PxTransform::createIdentity());
	collider->EnableTrigger(true);
	
	obj->AddComponent(new RigidBodyComponent(true));
	obj->AddComponent(collider);
	AddChild(obj);

	obj->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* receiver, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		UNREFERENCED_PARAMETER(receiver);
		UNREFERENCED_PARAMETER(action);

		//Handle Trigger Event :)
		Logger::LogInfo(L"TRIGGERED!");
	});
}

void CharacterTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

}

void CharacterTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
