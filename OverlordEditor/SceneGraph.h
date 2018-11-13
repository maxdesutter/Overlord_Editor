#pragma once
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();
	void Update(GameScene* scene);
	GameObject* GetSelectedObject() { return m_pSelectedObject; }

	void SetObjectPicking(GameObject* obj)
	{
		m_pSelectedObject = obj;
		m_SelectedSceneGraphNode = -1;

	}
	void DeselectGameObject()
	{
		m_SelectedSceneGraphNode = -1;
		m_pSelectedObject = nullptr;
	}
private:
	void createGraph(GameScene* scene);
	void DisplayGameObject(GameObject* obj, int& index, int& noNameIter);
	int m_Selection_mask_SceneGraph = (1 << 2);
	int m_SelectedSceneGraphNode = -1;
	GameObject* m_pSelectedObject = nullptr;
};

