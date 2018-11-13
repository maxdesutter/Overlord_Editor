
#include "stdafx.h"

#include "Wall.h"
#include "Components.h"
#include "PhysxManager.h"

Wall::Wall(float width, float height, float depth, bool isTrigger):
	m_Dimensions(width,height,depth),
	m_IsTrigger(isTrigger),
	m_WasTriggered(false)
{
}


Wall::~Wall(void)
{
}

void Wall::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto defaultMaterial = physX->createMaterial(0, 0, 1);


	auto rigidbody = new RigidBodyComponent(true);

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(m_Dimensions.x/2, m_Dimensions.y/2, m_Dimensions.z/2));
	auto collider = new ColliderComponent(geom,*defaultMaterial);

	if(m_IsTrigger) 
	{
		this->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* receiver, TriggerAction action)
											{
												UNREFERENCED_PARAMETER(receiver);
												UNREFERENCED_PARAMETER(trigger);
												if(action == TriggerAction::ENTER)this->m_WasTriggered = true;
											});
		collider->EnableTrigger(true);
	}
	AddComponent(rigidbody);

	AddComponent(collider);
}
