
#include "stdafx.h"

#include "ClothScene.h"
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

#define FPS_COUNTER 1

ClothScene::ClothScene(void):
	GameScene(L"ClothScene"),
	m_pClothObject(nullptr),
	m_pMeshDrawer(nullptr),
	m_pCloth(nullptr),
	m_ClothParticleCount(0)
{
}


ClothScene::~ClothScene(void)
{
}

void ClothScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	//GetPhysxProxy()->EnablePhysXFrameStepping(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto bouncyMaterial = physX->createMaterial(1.0f, 1.0f, 0.1f);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0,0,1)))));
	AddChild(ground);

	//CLOTH
	//*****

	auto clothWidth = 10.0f;
	auto clothHeight = 10.0f;
	auto clothWidthSteps = 20;
	auto clothHeightSteps = 20;
	auto offset = 0.0f;

	auto clothStartX = -clothWidth / 2.0f;
	auto clothStartY = clothHeight / 2.0f;
	auto clothStepX = clothWidth / clothWidthSteps;
	auto clothStepY = clothHeight / clothHeightSteps;

	auto vertexCount = clothWidthSteps * clothHeightSteps;
	m_ClothParticleCount = vertexCount;
	physx::PxClothParticle* particles = new physx::PxClothParticle[vertexCount];

	for (auto width = 0; width < clothWidthSteps; ++width)
	{
		for (auto height = 0; height < clothHeightSteps; ++height)
		{
			auto index = width*clothHeightSteps + height;
			particles[index] = physx::PxClothParticle(physx::PxVec3(clothStartX + (clothStepX * width), clothStartY - (clothStepY * height) + offset, 0.f), 1.0f);
		}
	}
	
	//for (int k = 5; k < 15; ++k)
	//	particles[k].invWeight = 0.f;

	auto indexCount = (clothWidthSteps - 1) * (clothHeightSteps - 1) * 6;
	physx::PxU32* indices = new physx::PxU32[indexCount];

	m_pMeshDrawer = new MeshIndexedDrawComponent(vertexCount, indexCount);

	auto currIndex = -1;
	for (auto x = 0; x < clothWidthSteps-1; ++x)
	{
		for (auto y = 0; y < clothHeightSteps-1; ++y)
		{
			auto v1 = x*clothWidthSteps + y;
			auto v2 = v1 + 1;
			auto v3 = v1 + clothWidthSteps;
			auto v4 = v3 + 1;

			indices[++currIndex] = v1;
			indices[++currIndex] = v2;
			indices[++currIndex] = v3;
			indices[++currIndex] = v3;
			indices[++currIndex] = v2;
			indices[++currIndex] = v4;

			m_pMeshDrawer->AddIndex(v1);
			m_pMeshDrawer->AddIndex(v2);
			m_pMeshDrawer->AddIndex(v3);

			m_pMeshDrawer->AddIndex(v3);
			m_pMeshDrawer->AddIndex(v2);
			m_pMeshDrawer->AddIndex(v4);
		}
	}

	physx::PxClothMeshDesc meshDesc;
	meshDesc.points.data = particles;
	meshDesc.points.count = vertexCount;
	meshDesc.points.stride = sizeof(physx::PxClothParticle);

	meshDesc.invMasses.data = &particles->invWeight;
	meshDesc.invMasses.count = vertexCount;
	meshDesc.invMasses.stride = sizeof(physx::PxClothParticle);

	meshDesc.triangles.data = indices;
	meshDesc.triangles.count = indexCount / 3;
	meshDesc.triangles.stride = sizeof(physx::PxU32) * 3;

	physx::PxClothFabric* fabric = PxClothFabricCreate(*physX, meshDesc, physx::PxVec3(0, -1, 0));
	

	physx::PxTransform pose = physx::PxTransform(physx::PxVec3(0.0f, 20.0f, 0.0f), physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(1, 0, 0)));
	m_pCloth = physX->createCloth(pose, *fabric, particles, physx::PxClothFlag::eSCENE_COLLISION);
	m_pCloth->setSimulationFilterData(physx::PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0));
	GetPhysxProxy()->GetPhysxScene()->addActor(*m_pCloth);

	physx::PxClothStretchConfig stretchConfig;
	stretchConfig.stiffness = 10.5f;
	stretchConfig.stiffnessMultiplier = 0.8f;
	stretchConfig.compressionLimit = 0.5f;
	stretchConfig.stretchLimit = 5.1f;
	m_pCloth->setStretchConfig(physx::PxClothFabricPhaseType::eVERTICAL, stretchConfig);


	//Renderer
	auto renderObj = new GameObject();
	renderObj->AddComponent(m_pMeshDrawer);
	AddChild(renderObj);
	m_pMeshDrawer->UpdateIndexBuffer();

	delete [] particles;
	delete [] indices;

	//CHAIR OBJECT
	//************
	auto pChair = new GameObject();

	//Chair Material
	auto chairMat = new DiffuseMaterial();
	chairMat->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	gameContext.pMaterialManager->AddMaterial(chairMat, 0);

	//Chair Model Component
	auto chairModel = new ModelComponent(L"./Resources/Meshes/Chair.ovm");
	chairModel->SetMaterial(0);
	pChair->AddComponent(chairModel);

	//Chair PhysX
	auto chairBody = new RigidBodyComponent();
	pChair->AddComponent(chairBody);

	auto chairPxMaterial = physX->createMaterial(0.5f, 0.5f, 0.1f);
	//auto chairPxMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Chair.ovpt");		
	//shared_ptr<physx::PxGeometry> chairGeom(new PxTriangleMeshGeometry(chairPxMesh));
	auto chairPxMesh = ContentManager::Load<physx::PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");
	std::shared_ptr<physx::PxGeometry> chairGeom(new physx::PxConvexMeshGeometry(chairPxMesh));
	auto chairCollider = new ColliderComponent(chairGeom, *chairPxMaterial);
	pChair->AddComponent(chairCollider);

	pChair->GetTransform()->Translate(0, 0.1f, 0);

	AddChild(pChair);
}

void ClothScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pMeshDrawer->ClearVertexList();
	auto particles = m_pCloth->lockParticleData();
	
	auto globalPose = m_pCloth->getGlobalPose();
	for (int i = 0; i < m_ClothParticleCount; ++i)
	{
		auto particle = particles->particles[i];
		auto transPos = globalPose.rotate(particle.pos);
		transPos += globalPose.p;
		m_pMeshDrawer->AddVertex(VertexPosNormCol(ToXMFLOAT3(transPos), DirectX::XMFLOAT3(0, 1, 0), (DirectX::XMFLOAT4) DirectX::Colors::Red));
	}

	particles->unlock();

	m_pMeshDrawer->GenerateNormals();
	m_pMeshDrawer->UpdateVertexBuffer();

	m_FpsInterval += gameContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	}
}

void ClothScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
