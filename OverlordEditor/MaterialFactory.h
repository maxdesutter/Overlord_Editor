#pragma once

class BinaryReader;
class MaterialFactory
{
public:
	MaterialFactory();
	~MaterialFactory();
	Material* CreateMaterialFromFile(BinaryReader* reader,GameScene* scene);
private:
	Material* CreateColorMatFromFile(BinaryReader* reader);
	Material* CreateDiffusematerialFromFile(BinaryReader* reader, GameScene* scene);
	Material* CreateSkinneddiffuseFromFile(BinaryReader* reader, GameScene* scene);
	Material* CreateSkyboxmatFromFile(BinaryReader* reader , GameScene* scne);
	Material* CreateSpikymatFromFile(BinaryReader* reader, GameScene* scne);
	Material* CreateUbermaterialFromFile(BinaryReader* reader, GameScene* scne);

};

