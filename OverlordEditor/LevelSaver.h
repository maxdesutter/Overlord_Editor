#pragma once
class LevelSaver
{
public:
	LevelSaver();
	~LevelSaver();
	void SaveToFile(std::wstring basepath,GameScene* scene);
private:
	void SaveMaterials(std::wstring basepath, GameScene* scene);
	void SaveModels(std::wstring basepath, GameScene* scene);
	void SavePhysxMaterials(std::wstring basepath, GameScene* scene);
	void SaveGameObjects(std::wstring basepath, GameScene* scene);
	void SaveTextures(std::wstring basepath, GameScene* scene);
};

