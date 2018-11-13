#pragma once
#include "Material.h"

class TextureData;

class SkyBoxMaterial : public Material
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial();

	void SetCubeMapTexture(const std::wstring& assetFile);
	void SetTextureStruct(TextureStruct* CubeTexture);
	TextureStruct* GetTextureStruct() { return m_pTextureStruct; }

	void WriteToFile(BinaryWriter* writer) override
	{
		writer->WriteLongString(GetName());
		writer->Write((unsigned char)3);
		writer->WriteLongString(m_TexPath);
	}
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	std::wstring m_TexPath;
	TextureData* m_pCubeTexture;
	static ID3DX11EffectShaderResourceVariable* m_pCubeSRVvariable;
	TextureStruct* m_pTextureStruct = nullptr;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxMaterial(const SkyBoxMaterial &obj);
	SkyBoxMaterial& operator=(const SkyBoxMaterial& obj);
};

