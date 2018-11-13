#include "stdafx.h"
#include "PostGrayscale.h"
#include "RenderTarget.h"

PostGrayscale::PostGrayscale()
	: PostProcessingMaterial(L"./Resources/Effects/Grayscale.fx", 1),
	m_pTextureMapVariabele(nullptr)
{
}

void PostGrayscale::LoadEffectVariables()
{
	m_pTextureMapVariabele = GetEffect()->GetVariableByName("gTexture" )->AsShaderResource();
}

void PostGrayscale::UpdateEffectVariables(RenderTarget* rendertarget)
{
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
}