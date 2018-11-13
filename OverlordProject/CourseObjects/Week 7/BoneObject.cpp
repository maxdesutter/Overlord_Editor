
#include "stdafx.h"

#include "BoneObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"


BoneObject::BoneObject(int boneId, int materialId, float length):
	m_Length(length),
	m_BoneId(boneId),
	m_MaterialId(materialId)
{

}


BoneObject::~BoneObject()
{
}


void BoneObject::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto emptyObj = new GameObject();

	auto model = new ModelComponent(L"./Resources/Meshes/Bone.ovm");
	model->SetMaterial(m_MaterialId);

	emptyObj->AddComponent(model);
	emptyObj->GetTransform()->Rotate(0, -90, 0);
	emptyObj->GetTransform()->Scale(m_Length, m_Length, m_Length);
	AddChild(emptyObj);
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length, 0, 0);
	AddChild(pBone);
}

//PART 2
void BoneObject::CalculateBindPose()
{
	auto childBones = GetChildren<BoneObject>();

	auto invWorld = XMMatrixInverse(nullptr, XMLoadFloat4x4(&GetTransform()->GetWorld()));
	XMStoreFloat4x4(&m_BindPose, invWorld);

	for (auto childBone : childBones)
	{
		childBone->CalculateBindPose();
	}
}