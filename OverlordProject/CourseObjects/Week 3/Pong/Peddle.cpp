
#include "stdafx.h"

#include "Peddle.h"
#include "Components.h"
#include "PhysxManager.h"

Peddle::Peddle(float width, float height, float depth, DirectX::XMFLOAT4 color):
	m_Dimensions(width,height,depth),
	m_Color(color)
{
}


Peddle::~Peddle(void)
{
}

void Peddle::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto defaultMaterial = physX->createMaterial(0, 0, 1);

	//Create Cube
	auto mesh = new MeshDrawComponent(12);
	
	float halfWidth = m_Dimensions.x/2.f;
	float halfHeight = m_Dimensions.y/2.f;
	float halfDepth = m_Dimensions.z/2.f;

	//FRONT
	mesh->AddQuad(
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, halfHeight, -halfDepth), DirectX::XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, halfHeight, -halfDepth), DirectX::XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT3(0, 0, -1), m_Color)
		);

	//BACK
	mesh->AddQuad(
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, halfHeight, halfDepth), DirectX::XMFLOAT3(0, 0, 1), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, halfHeight, halfDepth), DirectX::XMFLOAT3(0, 0, 1), m_Color),	
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, -halfHeight, halfDepth), DirectX::XMFLOAT3(0, 0, 1), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, -halfHeight, halfDepth), DirectX::XMFLOAT3(0, 0, 1), m_Color)	
		);

	//LEFT
	mesh->AddQuad(
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, halfHeight, halfDepth), DirectX::XMFLOAT3(-1, 0, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, halfHeight, -halfDepth), DirectX::XMFLOAT3(-1, 0, 0), m_Color),	
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT3(-1, 0, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, -halfHeight, halfDepth), DirectX::XMFLOAT3(-1, 0, 0), m_Color)	
		);

	//RIGHT
	mesh->AddQuad(
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, halfHeight, -halfDepth), DirectX::XMFLOAT3(1, 0, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, halfHeight, halfDepth), DirectX::XMFLOAT3(1, 0, 0), m_Color),	
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, -halfHeight, halfDepth), DirectX::XMFLOAT3(1, 0, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT3(1, 0, 0), m_Color)	
		);

	//TOP
	mesh->AddQuad(
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, halfHeight, halfDepth), DirectX::XMFLOAT3(0, 1, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, halfHeight, halfDepth), DirectX::XMFLOAT3(0, 1, 0), m_Color),	
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, halfHeight, -halfDepth), DirectX::XMFLOAT3(0, 1, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, halfHeight, -halfDepth), DirectX::XMFLOAT3(0, 1, 0), m_Color)	
		);

	//BOTTOM
	mesh->AddQuad(
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT3(0, -1, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, -halfHeight, -halfDepth), DirectX::XMFLOAT3(0, -1, 0), m_Color),	
		VertexPosNormCol(DirectX::XMFLOAT3(halfWidth, -halfHeight, halfDepth), DirectX::XMFLOAT3(0, -1, 0), m_Color),
		VertexPosNormCol(DirectX::XMFLOAT3(-halfWidth, -halfHeight, halfDepth), DirectX::XMFLOAT3(0, -1, 0), m_Color)	
		);

	AddComponent(mesh);


	auto rigidbody = new RigidBodyComponent();
	rigidbody->SetKinematic(true);
	AddComponent(rigidbody);

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxBoxGeometry(halfWidth, halfHeight, halfDepth));
	AddComponent(new ColliderComponent(geom,*defaultMaterial));
}
