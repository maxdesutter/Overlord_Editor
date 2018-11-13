#pragma once
class ContentBrowser
{
public:
	ContentBrowser();
	~ContentBrowser();

	void Update(GameScene* scene);
	Material* GetSelectedMaterial() { return m_SelectedMaterial; }
	PhysicsMaterialStruct* GetSelectedPhysxMaterial() { return m_selectedPhysxMat; }
	void Deselect()
	{
		m_SelectedMaterial = nullptr;
		m_selectedPhysxMat = nullptr;
	}
private:
	void ContentMenu(GameScene* scene);
	void ShowMaterials(const GameContext& gameContext);
	void ShowModels(GameScene* scene);
	void ShowBehaviorComponents(GameScene* scene);
	void ShowPhyscMaterials(GameScene* scene);
	void ShowTextures(GameScene* scene);
	Material* m_SelectedMaterial = nullptr;
	PhysicsMaterialStruct* m_selectedPhysxMat = nullptr;

};

