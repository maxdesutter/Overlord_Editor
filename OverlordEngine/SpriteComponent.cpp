#include "stdafx.h"

#include "SpriteComponent.h"
 #include <utility>

#include "GameObject.h"
#include "TextureData.h"
#include "ContentManager.h"
#include "SpriteRenderer.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(std::wstring spriteAsset, DirectX::XMFLOAT2 pivot, DirectX::XMFLOAT4 color):
	m_pTexture(nullptr),
	m_SpriteAsset(std::move(spriteAsset)),
	m_Pivot(pivot),
	m_Color(color)
{
	m_ComponentClassName = "SpriteComponent";

}

SpriteComponent::SpriteComponent(TextureStruct* spriteAsset, DirectX::XMFLOAT2 pivot, DirectX::XMFLOAT4 color) :
	m_pTexture(nullptr),
	m_Texture(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{
	m_ComponentClassName = "SpriteComponent";
	RegisterVariable<TextureStruct*>(this, &SpriteComponent::GetTextureStruct, &SpriteComponent::SetTextureStruct, "Texture");
}

void SpriteComponent::Initialize(const GameContext& )
{
	m_pTexture = ContentManager::Load<TextureData>(m_Texture->path);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::WriteToFile(BinaryWriter* writer)
{
	writer->Write((unsigned char)5);
	writer->WriteLongString(m_SpriteAsset);
	writer->Write(m_Pivot);
	writer->Write(m_Color);

}

void SpriteComponent::SetTextureStruct(TextureStruct* Texture)
{
	m_Texture = Texture;
	m_pTexture = ContentManager::Load<TextureData>(m_Texture->path);
}

void SpriteComponent::Update(const GameContext& )
{
}

void SpriteComponent::Draw(const GameContext& )
{
	if (!m_pTexture)
		return;

	const auto pos = GetTransform()->GetWorldPosition();
	const auto localPos = GetTransform()->GetPosition();
	const auto rot = GetTransform()->GetWorldRotation();
	const auto scale = GetTransform()->GetWorldScale();
	const float zRotation = QuaternionToEuler(rot).z;
	SpriteRenderer::GetInstance()->Draw(m_pTexture, DirectX::XMFLOAT2(pos.x, pos.y), m_Color, m_Pivot,
	                                    DirectX::XMFLOAT2(scale.x, scale.y), zRotation, localPos.z);
}
