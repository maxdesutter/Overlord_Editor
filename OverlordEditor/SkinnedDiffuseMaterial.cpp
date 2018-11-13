
#include "OverlordEditorPCH.h"


#include "SkinnedDiffuseMaterial.h"

#include "ContentManager.h"
#include "TextureData.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"

ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial::m_pBoneTransforms = nullptr;
ID3DX11EffectScalarVariable* SkinnedDiffuseMaterial::m_pHasBones =nullptr;

SkinnedDiffuseMaterial::SkinnedDiffuseMaterial() : Material(L"./../OverlordProject/Resources/Effects/PosNormTex3D_Skinned.fx"),
	m_pDiffuseTexture(nullptr)
{
	m_MaterialTypeName = L"SkinnedDiffuse";

	RegisterVariable<TextureStruct*>(this, &SkinnedDiffuseMaterial::GetDiffuseTextureStruct, &SkinnedDiffuseMaterial::SetDiffuseTextureStruct, "Texture");
}


SkinnedDiffuseMaterial::~SkinnedDiffuseMaterial()
{

}

void SkinnedDiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_TexPath = assetFile;
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkinnedDiffuseMaterial::SetDiffuseTextureStruct(TextureStruct* tex)
{
	m_TextureStruct = tex;
	m_pDiffuseTexture = ContentManager::Load<TextureData>(m_TextureStruct->path);
	
}

void SkinnedDiffuseMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	if (!m_pBoneTransforms)
	{
		m_pBoneTransforms = GetEffect()->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransforms->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gBones\' variable not found!");
			m_pBoneTransforms = nullptr;
		}
	}
	if (!m_pHasBones)
	{
		m_pHasBones = GetEffect()->GetVariableByName("gHasBones")->AsScalar();
		if (!m_pHasBones->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'m_pHasBones\' variable not found!");
			m_pHasBones = nullptr;
		}
	}
}

void SkinnedDiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	if (m_pBoneTransforms)
	{
		auto anim  = pModelComponent->GetAnimator();
		if (anim)
		{
			auto transforms = anim->GetBoneTransforms();

			//m_pBoneTransforms->SetRawValue((void*)transforms.data(), 0, transforms.size()*sizeof(DirectX::XMFLOAT4X4));
			m_pBoneTransforms->SetMatrixArray((float*)transforms.data(), 0, transforms.size());
			m_HasBones = true;

		}
		else
		{
			m_HasBones = false;
		}
		
	}
	if(m_pHasBones)
	{
		m_pHasBones->SetBool(m_HasBones);
	}
}