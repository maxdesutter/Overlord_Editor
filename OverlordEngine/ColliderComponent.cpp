#include "stdafx.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"

ColliderComponent::ColliderComponent(std::shared_ptr<physx::PxGeometry>& geometry, physx::PxMaterial& material,
                                     physx::PxTransform localPose) :
	m_Geometry(geometry),
	m_Material(material),
	m_LocalPose(std::move(localPose)),
	m_pShape(nullptr),
	m_isTrigger(false)
{
	m_ComponentClassName = "ColliderComponent";

	RegisterVariable<physx::PxGeometryType::Enum>(this, &ColliderComponent::GetType, &ColliderComponent::SetType, "Type");
	RegisterVariable<physx::PxTransform>(this, &ColliderComponent::GetLocalPose, &ColliderComponent::SetLocalPose, "Local Position");
	RegisterVariable<DirectX::XMFLOAT3>(this, &ColliderComponent::GetScale, &ColliderComponent::SetScale, "Scale");

}

ColliderComponent::ColliderComponent(std::shared_ptr<physx::PxGeometry>& geometry, PhysicsMaterialStruct* material,
	physx::PxTransform localPose):
	m_Geometry(geometry),
	m_pPxMatStruct(material),
	m_LocalPose(std::move(localPose)),
	m_pShape(nullptr),
	m_isTrigger(false),
	m_Material(*material->material)

{
	m_ComponentClassName = "ColliderComponent";

	RegisterVariable<physx::PxGeometryType::Enum>(this, &ColliderComponent::GetType, &ColliderComponent::SetType, "Type");
	RegisterVariable<physx::PxTransform>(this, &ColliderComponent::GetLocalPose, &ColliderComponent::SetLocalPose, "Local Position");
	RegisterVariable<DirectX::XMFLOAT3>(this, &ColliderComponent::GetScale, &ColliderComponent::SetScale, "Scale");
	RegisterVariable<PhysicsMaterialStruct*>(this, &ColliderComponent::Getmat, &ColliderComponent::SetMat, "material");
}


void ColliderComponent::Initialize(const GameContext&)
{
	
	auto rigidBody = GetGameObject()->GetComponent<RigidBodyComponent>();

	if (rigidBody == nullptr)
	{
		Logger::LogError(L"[ColliderComponent] Cannot add a Collider to an object that does not have a rigid body");
		return;
	}

	rigidBody->AddCollider(this);
	
}

void ColliderComponent::Update(const GameContext&)
{
}

void ColliderComponent::Draw(const GameContext&)
{
}

void ColliderComponent::EnableTrigger(const bool isTrigger)
{
	m_isTrigger = isTrigger;
	UpdateTriggerState();
}


void ColliderComponent::SetShape(physx::PxShape* shape)
{
	m_pShape = shape;
	UpdateTriggerState();
	
}

void ColliderComponent::UpdateTriggerState() const
{
	if (m_pShape != nullptr)
	{
		m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
		m_pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
	}
}

void ColliderComponent::SetType(physx::PxGeometryType::Enum geom)
{
	auto rigidBody = GetGameObject()->GetComponent<RigidBodyComponent>();
	rigidBody->RemoveCollider(this);

	switch (geom)
	{
	case physx::PxGeometryType::eSPHERE:
		{
		m_Geometry = std::shared_ptr<physx::PxGeometry>(new physx::PxSphereGeometry(1));

		}break;
	case physx::PxGeometryType::ePLANE:
		{
		rigidBody->SetKinematic(true);
		m_Geometry = std::shared_ptr<physx::PxGeometry>(new physx::PxPlaneGeometry());

		} break;
	case physx::PxGeometryType::eCAPSULE:
		{
		m_Geometry = std::shared_ptr<physx::PxGeometry>(new physx::PxCapsuleGeometry(1,2));

		}break;
	case physx::PxGeometryType::eBOX:
		{
		m_Geometry = std::shared_ptr<physx::PxGeometry>(new physx::PxBoxGeometry(1,1,1));

		} break;
	case physx::PxGeometryType::eCONVEXMESH:
		{
			
		} break;
	default: 
		return;
	}



	rigidBody->AddCollider(this);


}

physx::PxGeometryType::Enum ColliderComponent::GetType()
{
	return m_Geometry->getType();
}
void ColliderComponent::SetLocalPose(physx::PxTransform pose)
{
	m_LocalPose = pose;
	m_pShape->setLocalPose(m_LocalPose);




}

void ColliderComponent::SetScale(DirectX::XMFLOAT3 scale)
{
	if (scale.x <= 0) scale.x = 1;
	if (scale.y <= 0) scale.y = 1;
	if (scale.z <= 0) scale.z = 1;

	m_Scale = scale;

	switch (m_Geometry->getType())
	{
	case physx::PxGeometryType::eSPHERE:
	{
		m_Scale.y = 0;
		m_Scale.z = 0;
		((physx::PxSphereGeometry*)(m_Geometry.get()))->radius = m_Scale.x;
		m_pShape->setGeometry(*m_Geometry);


	}break;
	case physx::PxGeometryType::eCAPSULE:
	{
		m_Scale.z = 0;
		((physx::PxCapsuleGeometry*)(m_Geometry.get()))->radius = m_Scale.x;
		((physx::PxCapsuleGeometry*)(m_Geometry.get()))->halfHeight = m_Scale.y;

		m_pShape->setGeometry(*m_Geometry);
	} break;
	case physx::PxGeometryType::eBOX:
	{
		((physx::PxBoxGeometry*)(m_Geometry.get()))->halfExtents.x = scale.x;
		((physx::PxBoxGeometry*)(m_Geometry.get()))->halfExtents.y = scale.y;
		((physx::PxBoxGeometry*)(m_Geometry.get()))->halfExtents.z = scale.z;
		m_pShape->setGeometry(*m_Geometry);

	} break;
	case physx::PxGeometryType::eCONVEXMESH:
	{
		((physx::PxConvexMeshGeometry *)(m_Geometry.get()))->scale.scale = { m_Scale.x, m_Scale.y, m_Scale.z };
		m_pShape->setGeometry(*m_Geometry);
	} break;
	default:;
	}
}
PhysicsMaterialStruct* ColliderComponent::Getmat()
{
	return m_pPxMatStruct;
}

void ColliderComponent::SetMat(PhysicsMaterialStruct* ptr)
{
	m_pShape->setMaterials(&ptr->material, 1);
	m_pPxMatStruct = ptr;
}

void ColliderComponent::WriteToFile(BinaryWriter* writer)
{
	writer->Write((unsigned char)4);
	std::wstring name = *((std::wstring*)m_Material.userData);
	writer->WriteLongString(name);
	switch(m_Geometry->getType())
	{
	case physx::PxGeometryType::ePLANE:
		{
			writer->Write((unsigned char)0);
		}
		break;
	case physx::PxGeometryType::eSPHERE:
		{
			writer->Write((unsigned char)1);
			writer->Write(((physx::PxSphereGeometry*)m_Geometry.get())->radius);
		}
		break;
	case physx::PxGeometryType::eBOX:
		{
		writer->Write((unsigned char)2);
		writer->Write(((physx::PxBoxGeometry*)m_Geometry.get())->halfExtents.x);
		writer->Write(((physx::PxBoxGeometry*)m_Geometry.get())->halfExtents.y);
		writer->Write(((physx::PxBoxGeometry*)m_Geometry.get())->halfExtents.z);
		}
		break;
	case physx::PxGeometryType::eCONVEXMESH:
		{
		writer->Write((unsigned char)3);
		writer->WriteLongString(m_ConvexMeshPath);
		}
		break;
	default:
		Logger::Log(LogLevel::Error, L"Collidercomponent : WriteToFile - trying to write unsuported type");
	}
	writer->Write(m_LocalPose);

}

