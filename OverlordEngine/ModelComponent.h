#pragma once
#include "BaseComponent.h"
#include "DirectXCollision.h"

class MeshFilter;
class Material;
class ModelAnimator;

class ModelComponent : public BaseComponent
{
	MeshFilter* m_pMeshFilter = nullptr;
	ModelAnimator* m_pAnimator = nullptr;
	Material* m_pMaterial = nullptr;

	ModelReflectionStruct* m_ModelStruct = nullptr;
	unsigned int m_MaterialId = UINT_MAX;
	unsigned int m_NextMaterialID = UINT_MAX;

	bool m_MaterialUpdate = false;
	bool m_ModelUpdate = false;

	
	void UpdateMaterial(const GameContext& gameContext);
	void UpdateModel(const GameContext& gameContext);
	void WriteToFile(BinaryWriter*) override;

	ReflectionMaterialStruct GetMaterialStruct();
	void SetMaterialStruct(ReflectionMaterialStruct str);

	DirectX::BoundingOrientedBox m_box;

	void UpdateBoundingBox();
	void SetModelStruct(ModelReflectionStruct* str);
	ModelReflectionStruct* GetModelStruct() { return m_ModelStruct; }

public:
	ModelComponent(const ModelComponent& other) = delete;
	ModelComponent(ModelComponent&& other) noexcept = delete;
	ModelComponent& operator=(const ModelComponent& other) = delete;
	ModelComponent& operator=(ModelComponent&& other) noexcept = delete;
	ModelComponent(std::wstring  assetFile);
	ModelComponent(ModelReflectionStruct*  modelstruct);

	virtual ~ModelComponent();

	void SetMaterial(UINT materialId);
	UINT GetMaterialId();
	ModelAnimator* GetAnimator() const { return m_pAnimator; }
	bool HasAnimator() const { return m_pAnimator != nullptr; }


	float Intersects(const DirectX::XMVECTOR& origings , const DirectX::XMVECTOR& directions,float distance);
	DirectX::BoundingOrientedBox GetBoundingBox();
protected:
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void Initialize(const GameContext& gameContext) override;


};
