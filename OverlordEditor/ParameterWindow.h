#pragma once
class ComponentFactory;
class ObjectParameters;
class MaterialParameters;
class ParameterWindow
{
public:
	ParameterWindow();
	~ParameterWindow();

	void Update(GameScene* scene, GameObject* Object,  Material* material,  PhysicsMaterialStruct* pxMat);
private:
	ObjectParameters* m_ObjectParameters = nullptr;
	MaterialParameters* m_MaterialParameters = nullptr;
};

