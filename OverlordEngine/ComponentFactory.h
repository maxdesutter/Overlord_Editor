#pragma once
#include "ReflectionStructs.h"
#include "BehaviorComponentStructs.h"


class BinaryReader;
class ComponentFactory
{
public:
	ComponentFactory();
	~ComponentFactory();


	//scripts
	BaseComponent* CreateScriptComp(BinaryReader* reader, std::vector<BaseBehaviorComponentStruct*>* scripts);


	//particle

	BaseComponent* CreateFromFileParticleEmmiterComponent(BinaryReader* reader, std::vector<TextureStruct*>*  textures);

	BaseComponent* CreateBaseParticleEmmiter(TextureStruct* baseText);



	BaseComponent* CreateFromFileTerrainComponent(BinaryReader* reader);

	//spites
	BaseComponent* CreateFromFileSpriteComponent(BinaryReader* reader, std::vector<TextureStruct*>*  textures);
	BaseComponent* CreateBaseSpriteComponent(TextureStruct* baseText);


	//controller
	BaseComponent* CreateFromFileControllerComponent(BinaryReader* reader, const std::vector<PhysicsMaterialStruct*> &mats);
	BaseComponent* CreateControllerComponent( const std::vector<PhysicsMaterialStruct*> &mats);


	//model
	BaseComponent* CreateFromFileModelComponent(BinaryReader* reader,std::vector<ModelReflectionStruct*>* models);
	BaseComponent* CreateModelComponent(std::wstring path, UINT MaterialId);
	BaseComponent* CreateModelComponent(ModelReflectionStruct* str);

	//rigid
	BaseComponent* CreateFromFileRigidBOdyComponent(BinaryReader* reader);
	BaseComponent* CreateRigidBodyComponent();

	//collider
	BaseComponent* CreateFromFileColliderComponent(BinaryReader* reader, const std::vector<PhysicsMaterialStruct*> &mats);
	BaseComponent* CreateColliderComponent(const std::vector<PhysicsMaterialStruct*> &mats);

};

