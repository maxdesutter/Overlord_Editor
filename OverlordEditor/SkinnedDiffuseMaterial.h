#pragma once
#include "Material.h"

class TextureData;
class SkinnedDiffuseMaterial : public Material
{
public:
	SkinnedDiffuseMaterial();
	~SkinnedDiffuseMaterial();

	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetDiffuseTextureStruct(TextureStruct* tex);
	TextureStruct* GetDiffuseTextureStruct() { return m_TextureStruct; }

	void WriteToFile(BinaryWriter* writer) override
	{
		writer->WriteLongString(GetName());
		writer->Write((unsigned char)2);
		writer->WriteString(m_TextureStruct->name);
	}
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	std::wstring m_TexPath;
	TextureData* m_pDiffuseTexture;

	TextureStruct* m_TextureStruct = nullptr;;

	bool m_HasBones = false;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	static ID3DX11EffectMatrixVariable* m_pBoneTransforms;

	static ID3DX11EffectScalarVariable* m_pHasBones;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkinnedDiffuseMaterial(const SkinnedDiffuseMaterial &obj);
	SkinnedDiffuseMaterial& operator=(const SkinnedDiffuseMaterial& obj);
};

