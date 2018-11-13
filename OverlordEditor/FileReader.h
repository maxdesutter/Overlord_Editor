#pragma once

class MaterialFactory;
class FileReader
{
public:
	FileReader();
	~FileReader();
	void ReadLevel(std::wstring basepast, GameScene* scene);

	void ReadMaterials(std::wstring basepast, GameScene* scene);
	void ReadModel(std::wstring basepast, GameScene* scene);
	void ReadPhysxMaterials(std::wstring basepast, GameScene* scene);
	void ReadTextures(std::wstring basepast, GameScene* scene);
private:


	MaterialFactory* m_MatFactory = nullptr;
};

