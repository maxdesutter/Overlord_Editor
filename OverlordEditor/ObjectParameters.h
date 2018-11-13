#pragma once
class ComponentFactory;

class Exposer;
class ObjectParameters
{
public:
	ObjectParameters();
	~ObjectParameters();

	void Update(GameScene* scene, GameObject* Object);
private:
	void Rename(GameObject* Object);
	void ParameterMenuBar(GameScene* scene, GameObject* Object);
	bool ShouldShowPopUp();
	void ShowPopUp();
	void TransformComponentPart(GameObject* Object);
	void ExposeRegisteredComponentVariables(std::vector<BaseComponent*>*allComponents, const GameContext& gameContext);
	void AddGameComponent(GameObject* Object);

	ComponentFactory* m_CompFactory;
	Exposer* m_Exposer;

	BaseComponent* m_RightClickedComponent = nullptr;
	float TextInputRotation[6]{ 0,0,0,0,0,0 };

	float m_StandardItemWidth = 60;
};

