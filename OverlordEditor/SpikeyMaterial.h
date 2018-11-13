#pragma once
#include "Material.h"

class TextureData;

class SpikeyMaterial : public Material
{
public:
	SpikeyMaterial();
	~SpikeyMaterial();

	void SetTextureStruct(TextureStruct* str);


	TextureStruct* GetTextureStruct() { return m_textureStruct; }
	void SetDiffuseTexture(const std::wstring& assetFile);
	void WriteToFile(BinaryWriter* writer) override
	{
		writer->WriteLongString(GetName());
		writer->Write((unsigned char)4);
		writer->WriteLongString(m_TexPath);
	}
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

	TextureStruct* m_textureStruct = nullptr;
private:
	std::wstring m_TexPath;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpikeyMaterial(const SpikeyMaterial &obj);
	SpikeyMaterial& operator=(const SpikeyMaterial& obj);
};

