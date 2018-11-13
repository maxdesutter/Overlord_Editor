
#include "stdafx.h"

#include "ModelTestScene.h"

#include "GameObject.h"
#include "ModelComponent.h"
#include "../../Materials/ColorMaterial.h"
#include "../../Materials/DiffuseMaterial.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "ContentManager.h"
#include "PhysxManager.h"
#include "TransformComponent.h"


ModelTestScene::ModelTestScene(void) :
GameScene(L"ModelTestScene"),
m_pChair(nullptr)
{
}


ModelTestScene::~ModelTestScene(void)
{
}

void ModelTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(ground);

	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	//Chair Material
	auto chairMat = new DiffuseMaterial();
	chairMat->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	gameContext.pMaterialManager->AddMaterial(chairMat, 0);

	//Chair Model Component
	auto chairModel = new ModelComponent(L"./Resources/Meshes/Chair.ovm");
	chairModel->SetMaterial(0);
	m_pChair->AddComponent(chairModel);

	//Chair PhysX
	auto chairBody = new RigidBodyComponent();
	m_pChair->AddComponent(chairBody);

	auto chairPxMaterial = physX->createMaterial(0.5f, 0.5f, 0.1f);
	//auto chairPxMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Chair.ovpt");		
	//shared_ptr<physx::PxGeometry> chairGeom(new PxTriangleMeshGeometry(chairPxMesh));
	auto chairPxMesh = ContentManager::Load<physx::PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");
	auto chairPxMesh1 = ContentManager::Load<physx::PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");
	auto chairPxMesh2 = ContentManager::Load<physx::PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");
	UNREFERENCED_PARAMETER(chairPxMesh1);
	UNREFERENCED_PARAMETER(chairPxMesh2);

	std::shared_ptr<physx::PxGeometry> chairGeom(new physx::PxConvexMeshGeometry(chairPxMesh));

	auto chairCollider = new ColliderComponent(chairGeom, *chairPxMaterial);
	m_pChair->AddComponent(chairCollider);

	m_pChair->GetTransform()->Translate(0, 10, 0);

	AddChild(m_pChair);

	//m_pChair = new GameObject();

	////Chair Material
	//auto chairMat2 = new ColorMaterial();
	////chairMat->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	//gameContext.pMaterialManager->AddMaterial(chairMat2, 1);

	////Chair Model Component
	//chairModel = new ModelComponent(L"./Resources/Meshes/Chair.ovm");
	//chairModel->SetMaterial(1);
	//m_pChair->AddComponent(chairModel);
	//AddChild(m_pChair);


}

void ModelTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ModelTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
