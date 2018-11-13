#include "stdafx.h"
#include "ModelComponent.h"
#include "ContentManager.h"
#include "MeshFilter.h"
#include "Material.h"
#include "ModelAnimator.h"
#include "OverlordGame.h"
#include "TransformComponent.h"
ModelComponent::ModelComponent(std::wstring assetFile)
{
	Logger::Log(LogLevel::Warning, L"trying to create modelcomponent from file path is deprectated and disabled");
	//m_ModelStruct.Path = std::move(assetFile);
	m_ComponentClassName = "ModelComponent";
	//RegisterVariable<ReflectionMaterialStruct>(this, &ModelComponent::GetMaterialStruct, &ModelComponent::SetMaterialStruct, "Material");
}

ModelComponent::ModelComponent(ModelReflectionStruct* modelstruct)
{
	m_ModelStruct = modelstruct;
	m_ComponentClassName = "ModelComponent";
	
	RegisterVariable<ReflectionMaterialStruct>(this, &ModelComponent::GetMaterialStruct, &ModelComponent::SetMaterialStruct, "Material");	
	RegisterVariable<ModelReflectionStruct*>(this, &ModelComponent::GetModelStruct, &ModelComponent::SetModelStruct, "Model");
	RegisterGetter<ModelAnimator*>(this, &ModelComponent::GetAnimator, "animator");


}

ModelComponent::~ModelComponent()
{
	SafeDelete(m_pAnimator);
}

void ModelComponent::Initialize(const GameContext& gameContext)
{
	m_pMeshFilter = ContentManager::Load<MeshFilter>(m_ModelStruct->Path);
	m_pMeshFilter->BuildIndexBuffer(gameContext);


	if (m_pMeshFilter->m_HasAnimations)
		m_pAnimator = new ModelAnimator(m_pMeshFilter);


	UpdateMaterial(gameContext);
	UpdateBoundingBox();
};

inline void ModelComponent::UpdateMaterial(const GameContext& gameContext)
{

	if (m_MaterialUpdate)
	{
		m_MaterialUpdate = false;

		//FORWARD
		const auto mat = gameContext.pMaterialManager->GetMaterialNoError(m_NextMaterialID);
		if (mat == nullptr)
		{
			Logger::LogFormat(LogLevel::Warning, L"ModelComponent::UpdateMaterial > Material with ID \"%i\" doesn't exist!",
				m_NextMaterialID);
			m_NextMaterialID = m_MaterialId;
			return;
		}
		m_MaterialId = m_NextMaterialID;

		m_pMaterial = mat;
		m_pMaterial->Initialize(gameContext);
		m_pMeshFilter->BuildVertexBuffer(gameContext, m_pMaterial);

		if (OverlordGame::GetGameSettings().DirectX.EnableDeferredRendering)
		{
			auto matDeferred = gameContext.pMaterialManager->GetMaterial(m_MaterialId, true);
			if (matDeferred == nullptr)
			{
				Logger::LogFormat(LogLevel::Warning,
				                  L"ModelComponent::UpdateMaterial > No Deferred Material with ID \"%i\"! [DEFERRED MODE: ON]",
				                  m_MaterialId);
				return;
			}

			matDeferred->Initialize(gameContext);
		}

		
	}
}

void ModelComponent::UpdateModel(const GameContext& gameContext)
{

	if(m_ModelUpdate)
	{
		m_ModelUpdate = false;

		m_pMeshFilter = ContentManager::Load<MeshFilter>(m_ModelStruct->Path);
		m_pMeshFilter->BuildIndexBuffer(gameContext);


		if (m_pMeshFilter->m_HasAnimations)
		{
			delete m_pAnimator;
			m_pAnimator = new ModelAnimator(m_pMeshFilter);
		}
		else
		{
			if(m_pAnimator)
			{
				delete m_pAnimator;
				m_pAnimator = nullptr;
			}
		}
		UpdateMaterial(gameContext);
		UpdateBoundingBox();

	}
}



void ModelComponent::Update(const GameContext& gameContext)
{

	if (m_pAnimator)
	{
		m_pAnimator->Update(gameContext);
	}
};

void ModelComponent::Draw(const GameContext& gameContext)
{
	UpdateMaterial(gameContext);
	UpdateModel(gameContext);
	if (!m_pMaterial)
	{
		Logger::LogWarning(L"ModelComponent::Draw() > No Material!");
		return;
	}

	m_pMaterial = gameContext.pMaterialManager->GetMaterial(m_MaterialId);
	m_pMaterial->SetEffectVariables(gameContext, this);

	//Set Inputlayout
	gameContext.pDeviceContext->IASetInputLayout(m_pMaterial->GetInputLayout());

	//Set Vertex Buffer
	UINT offset = 0;
	auto vertexBufferData = m_pMeshFilter->GetVertexBufferData(gameContext, m_pMaterial);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride,
	                                               &offset);
	
	//Set Index Buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	//Set Primitive Topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DRAW
	auto tech = m_pMaterial->GetTechnique();
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_pMeshFilter->m_IndexCount, 0, 0);

	}

	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&GetTransform()->GetWorld());
	DirectX::BoundingOrientedBox newbox;
	m_box.Transform(newbox, world);


	

	
};

void ModelComponent::SetMaterial(unsigned int materialId)
{
	m_MaterialUpdate = true;

	m_NextMaterialID = materialId;
}

UINT ModelComponent::GetMaterialId()
{
	return m_MaterialId;
}
void ModelComponent::WriteToFile(BinaryWriter* writer)
{
	writer->Write((unsigned char)1);
	writer->WriteString(m_ModelStruct->modelName);
	writer->Write((UINT)m_MaterialId);
}


//refletion
ReflectionMaterialStruct ModelComponent::GetMaterialStruct()
{
		return  ReflectionMaterialStruct(m_MaterialId);
}
void ModelComponent::SetMaterialStruct(ReflectionMaterialStruct str)
{
	m_MaterialUpdate = true;

	m_NextMaterialID = str.materialID;
}

float ModelComponent::Intersects(const DirectX::XMVECTOR& origings, const DirectX::XMVECTOR& directions, float distance)
{

	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&GetTransform()->GetWorld());

	
	DirectX::BoundingOrientedBox newbox;
	m_box.Transform(newbox, world);

	
	if (!newbox.Intersects(origings, directions, distance))
	{
		return 0;
	}
	DirectX::XMFLOAT3 originFloat;
	DirectX::XMStoreFloat3(&originFloat, origings);
	return sqrt(pow(originFloat.x - newbox.Center.x, 2) + pow(originFloat.y - newbox.Center.y, 2) + pow(originFloat.z - newbox.Center.z, 2));
}

DirectX::BoundingOrientedBox ModelComponent::GetBoundingBox()
{
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&GetTransform()->GetWorld());


	DirectX::BoundingOrientedBox newbox;
	m_box.Transform(newbox, world);
	return newbox;
}
void ModelComponent::UpdateBoundingBox()
{
	using namespace DirectX;

	//make bounding box

	auto positions = m_pMeshFilter->m_Positions;


	if (m_pAnimator)
	{
		
		std::vector<XMFLOAT4> boneWeights = m_pMeshFilter->m_BlendWeights;
		std::vector<XMFLOAT4> boneIndices = m_pMeshFilter->m_BlendIndices;
		std::vector<XMFLOAT4X4> BoneTransformations = m_pAnimator->GetBoneTransforms();
		for (size_t i = 0; i < positions.size(); i++)
		{


			DirectX::XMFLOAT4 originalPosition(positions[i].x, positions[i].y, positions[i].z, 1);
			DirectX::XMFLOAT4 transformedPosition(0, 0, 0, 0);

			for (int n = 0; n < 4; ++n)
			{
				int boneIndex = 0;
				switch (n)
				{
					case 0:
					{
						boneIndex = (int)boneIndices[i].x;
					}break;
					case 1:
					{
						boneIndex = (int)boneIndices[i].y;
					}break;
					case 2:
					{
						boneIndex = (int)boneIndices[i].z;
					}break;
					case 3:
					{
						boneIndex = (int)boneIndices[i].z;
					}break;
				}
				if (boneIndex > -1)
				{
					float boneWeight = 0;
					switch (n)
					{
						case 0:
						{
							boneWeight = boneWeights[i].x;
						}break;
						case 1:
						{
							boneWeight = boneWeights[i].y;
						}break;
						case 2:
						{
							boneWeight = boneWeights[i].z;
						}break;
						case 3:
						{
							boneWeight = boneWeights[i].w;
						}break;
					}
					DirectX::XMMATRIX boneMatrix = DirectX::XMLoadFloat4x4(&BoneTransformations[boneIndex]);

					XMFLOAT4 newpos;
					XMStoreFloat4(&newpos, DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&originalPosition), boneMatrix) * boneWeight);
					transformedPosition.x += newpos.x;
					transformedPosition.y += newpos.y;
					transformedPosition.z += newpos.z;
					transformedPosition.w += newpos.w;
				}



			}
			positions[i].x = transformedPosition.x;
			positions[i].y = transformedPosition.y;
			positions[i].z = transformedPosition.z;

		}

	}
	//get point furthest away
	float maxX = 0, maxY = 0, maxZ = 0,minx=0,miny=0,minz=0;
	DirectX::XMFLOAT3 furthest{ 0,0,0 };
	for (auto pos : positions)
	{
		if (pos.x > maxX)
		{
			maxX =pos.x;
		}
		if (pos.x < minx)
		{
			minx = pos.x;
		}

		if (pos.y > maxY)
		{
			maxY = pos.y;
		}
		if (pos.y < miny)
		{
			miny = pos.y;
		}

		if (pos.z > maxZ)
		{
			maxZ =pos.z;
		}
		if (pos.z < minz)
		{
			minz = pos.z;
		}
	}


	//Set box Values

	m_box.Center.x =( maxX + minx)/2;
	m_box.Center.y = (maxY + miny)/2;
	m_box.Center.z = (maxZ + minz)/2;


	m_box.Extents.x = ((abs(maxX) > abs(minx)) ? maxX : minx) - m_box.Center.x;
	m_box.Extents.y = ((abs(maxY) > abs(miny)) ? maxY : miny)- m_box.Center.y;
	m_box.Extents.z = ((abs(maxZ) > abs(minz)) ? maxZ : minz)- m_box.Center.z;
}

void ModelComponent::SetModelStruct(ModelReflectionStruct* str)
{
	m_ModelStruct = str;

	m_ModelUpdate = true;

}
