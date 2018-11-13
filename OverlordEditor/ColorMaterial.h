#pragma once
#include "Material.h"

class ColorMaterial: public Material
{
public:
	ColorMaterial(bool enableTransparency = false);
	~ColorMaterial();

	 void WriteToFile(BinaryWriter *writer)override
	 {
		 writer->WriteLongString(GetName());
		 writer->Write((unsigned char)0);
		 writer->Write(m_Color);
	 };
	 DirectX::XMFLOAT4 GetColor() { return m_Color; }
	 void SetColor(DirectX::XMFLOAT4 col) { m_Color = col; }
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	DirectX::XMFLOAT4 m_Color{ 1.f, 0.f, 0.f, 1.f };
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------

	static ID3DX11EffectVectorVariable* m_pColor;


	ColorMaterial(const ColorMaterial &obj);
	ColorMaterial& operator=(const ColorMaterial& obj);
};

