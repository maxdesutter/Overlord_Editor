#include "stdafx.h"
#include "PostBlur.h"
#include "RenderTarget.h"

PostBlur::PostBlur()
	: PostProcessingMaterial(L"./Resources/Effects/Blur.fx", 2),
	m_pTextureMapVariabele(nullptr)
{
}

void PostBlur::LoadEffectVariables()
{
	m_pTextureMapVariabele = GetEffect()->GetVariableByName("gTexture" )->AsShaderResource();
}

void PostBlur::UpdateEffectVariables(RenderTarget* rendertarget)
{
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
}