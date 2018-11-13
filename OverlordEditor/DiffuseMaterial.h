#pragma once
#include "Material.h"

class TextureData;

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial();
	~DiffuseMaterial();

	void SetDiffuseTexture(const std::wstring& assetFile);
	void WriteToFile(BinaryWriter* writer) override
	{
		writer->WriteLongString(GetName());
		writer->Write((unsigned char)1);
		writer->WriteLongString(m_TexPath);
	}

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	std::wstring m_TexPath;
	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DiffuseMaterial(const DiffuseMaterial &obj);
	DiffuseMaterial& operator=(const DiffuseMaterial& obj);
};

