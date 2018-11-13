#pragma once
#include "BaseComponent.h"

class ColliderComponent : public BaseComponent
{
	friend class RigidBodyComponent; // TODO: get rid of this ugly dependency

	std::shared_ptr<physx::PxGeometry> m_Geometry;
	physx::PxMaterial& m_Material;
	physx::PxTransform m_LocalPose;
	physx::PxShape* m_pShape;
	bool m_isTrigger;

	DirectX::XMFLOAT3 m_Scale{1, 1, 1};
	void SetShape(physx::PxShape* shape);
	void UpdateTriggerState() const;


	void SetType(physx::PxGeometryType::Enum geom);
	physx::PxGeometryType::Enum GetType();

	PhysicsMaterialStruct* m_pPxMatStruct;
	PhysicsMaterialStruct* Getmat();
	void SetMat(PhysicsMaterialStruct* material);
public:
	ColliderComponent(const ColliderComponent& other) = delete;
	ColliderComponent(ColliderComponent&& other) noexcept = delete;
	ColliderComponent& operator=(const ColliderComponent& other) = delete;
	ColliderComponent& operator=(ColliderComponent&& other) noexcept = delete;
	ColliderComponent(std::shared_ptr<physx::PxGeometry>& geometry,  physx::PxMaterial& material, physx::PxTransform localPose = physx::PxTransform::createIdentity());
	ColliderComponent(std::shared_ptr<physx::PxGeometry>& geometry, PhysicsMaterialStruct* material, physx::PxTransform localPose = physx::PxTransform::createIdentity());

	~ColliderComponent() = default;

	physx::PxShape* GetShape() const { return m_pShape; }
	bool IsTrigger() const { return m_isTrigger; }
	void EnableTrigger(bool isTrigger);

	void WriteToFile(BinaryWriter*) override;

	std::wstring m_ConvexMeshPath;

	physx::PxTransform GetLocalPose() { return m_LocalPose; }
	void SetLocalPose(physx::PxTransform pose);

	DirectX::XMFLOAT3 GetScale() { return m_Scale; }
	void SetScale(DirectX::XMFLOAT3 scale);


protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
};

