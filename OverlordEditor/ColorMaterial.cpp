#include "OverlordEditorPCH.h"
#include "ColorMaterial.h"
#include <winnt.h>

ID3DX11EffectVectorVariable* ColorMaterial::m_pColor = nullptr;

ColorMaterial::ColorMaterial(bool enableTransparency) : 
	Material(L"./Resources/Effects/PosNormCol3D.fx", enableTransparency?L"TransparencyTech":L"")
{
	m_MaterialTypeName = L"ColorMaterial";
	RegisterVariable<DirectX::XMFLOAT4>(this, &ColorMaterial::GetColor, &ColorMaterial::SetColor, "Color");
}


ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::LoadEffectVariables()
{
	if (!m_pColor)
	{
		m_pColor = GetEffect()->GetVariableByName("gColor")->AsVector();
		if (!m_pColor->IsValid())
		{
			Logger::LogWarning(L"ColorMaterial::LoadEffectVariables() > \'m_Color\' variable not found!");
			m_pColor = nullptr;
		}
	}
}

void ColorMaterial::UpdateEffectVariables(const GameContext& , ModelComponent* )
{
	if (m_pColor)
	{
		m_pColor->SetFloatVector(reinterpret_cast<float*>(&m_Color));
	}
}
