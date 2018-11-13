#pragma once
#include "GameScene.h"
#include "GeneralStructs.h"
#include "BehaviorComponentStructs.h"

class ComponentFactory;
class GUI;
class LevelSaver;
class FileReader;
class ColorMaterial;
class EditorScene:public GameScene
{
public:
	EditorScene(const EditorScene& other) = delete;
	EditorScene(EditorScene&& other) noexcept = delete;
	EditorScene& operator=(const EditorScene& other) = delete;
	EditorScene& operator=(EditorScene&& other) noexcept = delete;
	EditorScene();
	~EditorScene();



	void ReadFromFile(std::wstring filePath);
	void SaveToFile(std::wstring filepath);


	void ClearScene();
protected:

	void Initialize(const GameContext& gameContext);
	void Update(const GameContext& gameContext);
	void Draw(const GameContext& gameContext);


private:
	GUI* m_Gui = nullptr;

	ComponentFactory* m_ComponentCreater = nullptr;

	LevelSaver* m_Saver = nullptr;
	FileReader* m_Reader = nullptr;

	//default material
	ColorMaterial* m_pDefMat = nullptr;

};

