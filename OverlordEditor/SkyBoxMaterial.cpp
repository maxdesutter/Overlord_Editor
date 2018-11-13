
#include "OverlordEditorPCH.h"


#include "SkyBoxMaterial.h"

#include "ContentManager.h"
#include "TextureData.h"

ID3DX11EffectShaderResourceVariable* SkyBoxMaterial::m_pCubeSRVvariable = nullptr;

SkyBoxMaterial::SkyBoxMaterial() : Material(L"./Resources/Effects/SkyBox.fx"),
	m_pCubeTexture(nullptr)
{
	RegisterVariable<TextureStruct*>(this, &SkyBoxMaterial::GetTextureStruct, &SkyBoxMaterial::SetTextureStruct, "Texture");

}


SkyBoxMaterial::~SkyBoxMaterial()
{

}

void SkyBoxMaterial::SetCubeMapTexture(const std::wstring& assetFile)
{
	m_TexPath = assetFile;
	m_pCubeTexture = ContentManager::Load<TextureData>(assetFile);
}
void SkyBoxMaterial::SetTextureStruct(TextureStruct* CubeTexture)
{
	m_pTextureStruct = CubeTexture;
	m_pCubeTexture = ContentManager::Load<TextureData>(m_pTextureStruct->path);


}


void SkyBoxMaterial::LoadEffectVariables()
{
	if (!m_pCubeSRVvariable)
	{
		m_pCubeSRVvariable = GetEffect()->GetVariableByName("m_CubeMap")->AsShaderResource();
		if (!m_pCubeSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'m_CubeMap\' variable not found!");
			m_pCubeSRVvariable = nullptr;
		}
	}
}

void SkyBoxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pCubeTexture && m_pCubeSRVvariable)
	{
		m_pCubeSRVvariable->SetResource(m_pCubeTexture->GetShaderResourceView());
	}
}