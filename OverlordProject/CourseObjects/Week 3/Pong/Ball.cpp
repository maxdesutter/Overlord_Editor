
#include "stdafx.h"

#include "Ball.h"
#include "Components.h"
#include "PhysxManager.h"

Ball::Ball(float size, DirectX::XMFLOAT4 color)
	:
	m_Color(color),
	m_Size(size)
{
}


Ball::~Ball(void)
{
}

void Ball::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	// bouncy
	auto defaultMaterial = physX->createMaterial(0, 0, 1);


	auto rigidbody = new RigidBodyComponent();
	rigidbody->SetConstraint(RigidBodyConstraintFlag::TransY, true);
	AddComponent(rigidbody);

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxSphereGeometry(m_Size));
	AddComponent(new ColliderComponent(geom,*defaultMaterial));
}
