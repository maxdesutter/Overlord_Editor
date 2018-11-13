#pragma once
class Exposer;
class MaterialParameters
{
public:
	MaterialParameters();
	~MaterialParameters();

	void UpdateMaterials(GameScene* scene, Material* Object);
	void UpdatePhysxMaterials(GameScene* scene, PhysicsMaterialStruct* mat);
private:
	void RenameMaterial(Material* Object);
	void RenamePhysicMaterial(PhysicsMaterialStruct* mat);
	void ExposeParamterars(GameScene* scene, Material* Object);
	Exposer* m_Exposer = nullptr;
};

