#pragma once
#include "EffectHelper.h"
#include "BinaryWriter.h"

class MeshFilter;
class ModelComponent;
struct GameContext;

class Material
{
public:
	Material(std::wstring  effectFile, std::wstring  technique = L"");
	virtual ~Material();

	Material(const Material& other) = delete;
	Material(Material&& other) noexcept = delete;
	Material& operator=(const Material& other) = delete;
	Material& operator=(Material&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext);
	void SetEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);
	ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; }
	ID3D11InputLayout* GetInputLayout() const { return m_pInputLayout; }

	//write to file
	virtual void WriteToFile(BinaryWriter* ) {};


	void SetName(std::wstring name)
	{
		m_MaterialName = name;
	}
	std::wstring GetName()
	{
		return m_MaterialName;
	}
	std::wstring GetTypeName()
	{
		return m_MaterialTypeName;
	}
	std::vector<GetSetinterface*>* GetRegisteredMethods() { return  &m_GetSetMethods; }

protected:

	virtual void LoadEffectVariables() = 0;
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) = 0;

	std::wstring m_MaterialTypeName = L"baseMaterial";

	ID3DX11Effect* GetEffect() const { return m_pEffect; }

	//variable registry
	template<class Type, typename OBJECT, typename GetterFunc, typename SetterFunc>
	void RegisterVariable(OBJECT obj, GetterFunc getterF, SetterFunc setterF, std::string name)
	{
		std::function<void(Type)> setter = std::bind(setterF, obj, std::placeholders::_1);
		std::function<Type()> getter = std::bind(getterF, obj);


		GetSetMethods<Type> *meths = new GetSetMethods<Type>;

		meths->varName = name;
		meths->Set = setter;
		meths->Get = getter;
		meths->type = GetReflectionType<Type>();

		m_GetSetMethods.push_back(meths);

	}


private:
	// TODO: get rid of this nasty dependency
	friend class MeshFilter;

	bool LoadEffect(const GameContext& gameContext);

	std::wstring m_effectFile;
	ID3DX11Effect* m_pEffect;

	std::wstring m_TechniqueName;
	ID3DX11EffectTechnique* m_pTechnique;

	ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable;
	ID3DX11EffectMatrixVariable* m_pViewMatrixVariable;
	ID3DX11EffectMatrixVariable* m_pViewInverseMatrixVariable;
	ID3DX11EffectMatrixVariable* m_pWvpMatrixVariable;

	ID3D11InputLayout* m_pInputLayout;
	std::vector<ILDescription> m_pInputLayoutDescriptions;
	unsigned int m_pInputLayoutSize;
	unsigned int m_InputLayoutID;



	std::wstring m_MaterialName;
	bool m_IsInitialized;

	std::vector<GetSetinterface*> m_GetSetMethods;
};
