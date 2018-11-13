#pragma once
#include "ReflectionStructs.h"

class BinaryReader;
class GameObject;
class EditorScene;
class ComponentFactory;
class GameObjectFactory
{
public:
	GameObjectFactory();
	~GameObjectFactory();
	GameObject* CreateStandardGameObject(GameScene* scene);

	GameObject* CreateGameObjectFromFile(BinaryReader* reader, GameScene* scene, GameObject* parent = nullptr);
	GameObject* CreateGameObject();
private:
	//components
		//modelComponent

	ComponentFactory* m_CompFactory = nullptr;
	
};

