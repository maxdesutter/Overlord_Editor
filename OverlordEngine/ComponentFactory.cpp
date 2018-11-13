#include "stdafx.h"
#include "ComponentFactory.h"
#include "Components.h"
#include "BinaryReader.h"
#include "ContentManager.h"
#include "BaseBehaviorComponent.h"

ComponentFactory::ComponentFactory()
{
}


ComponentFactory::~ComponentFactory()
{
}

BaseComponent* ComponentFactory::CreateScriptComp(BinaryReader* reader, std::vector<BaseBehaviorComponentStruct*>* scripts)
{
	std::string name = reader->ReadRegString();
	for(auto scr :*scripts)
	{
		if (scr->name == name)
		{
			auto scrc = (BaseBehaviorComponent*)scr->Getnew();
			scrc->ReadinParameters(reader);
			return scrc;
		}
	}
	return nullptr;
}

BaseComponent* ComponentFactory::CreateFromFileControllerComponent(BinaryReader* reader,	const std::vector<PhysicsMaterialStruct*>& mats)
{
	std::wstring namePmat = reader->ReadLongString();
	physx::PxMaterial* matPtr = nullptr;
	for (auto mat : mats)
	{
		if (mat->materialName == namePmat)
		{
			matPtr = mat->material;
		}
	}
	if (matPtr == nullptr)
	{
		Logger::Log(LogLevel::Error, L"GameObjectFactory Trying to create Controllercomponent with invalid Pxmaterial :"
			L"with materialname:" + namePmat);
		return nullptr;
	}

	ControllerComponent* contComp = new ControllerComponent(matPtr, reader->Read<float>(), reader->Read<float>());
	return contComp;
}

BaseComponent* ComponentFactory::CreateControllerComponent(const std::vector<PhysicsMaterialStruct*>& mats)

{

	ControllerComponent* contComp = new ControllerComponent(mats[0]->material,1, 1);
	return contComp;
}

BaseComponent* ComponentFactory::CreateFromFileRigidBOdyComponent(BinaryReader* reader)
{
	RigidBodyComponent* rigComp = new RigidBodyComponent(reader->Read<bool>());
	rigComp->SetKinematic(reader->Read<bool>());
	return rigComp;
}
BaseComponent* ComponentFactory::CreateRigidBodyComponent()
{
	RigidBodyComponent* rigComp = new RigidBodyComponent(false);
	return rigComp;
}


//collider components
BaseComponent* ComponentFactory::CreateFromFileColliderComponent(BinaryReader* reader,
	const std::vector<PhysicsMaterialStruct*>& mats)
{
	std::wstring namePmat = reader->ReadLongString();
	PhysicsMaterialStruct* matPtr = nullptr;
	for (auto mat : mats)
	{
		if (mat->materialName == namePmat)
		{
			matPtr = mat;
		}
	}
	if (matPtr == nullptr)
	{
		Logger::Log(LogLevel::Error, L"GameObjectFactory Trying to create ColliderComponent with invalid Pxmaterial :"
			L"with materialname:" + namePmat);
		return nullptr;
	}

	unsigned char colType = reader->Read<unsigned char>();
	ColliderComponent* colComp = nullptr;
	//convex
	bool isConvex = false;
	std::wstring convexPath;
	std::shared_ptr<physx::PxGeometry> geom;
	switch (colType)
	{
	case 0:
	{
		geom = std::shared_ptr<physx::PxGeometry>(new physx::PxPlaneGeometry());

	}
	break;
	case 1:
	{
		geom = std::shared_ptr<physx::PxGeometry>(new physx::PxSphereGeometry(reader->Read<physx::PxReal>()));
	}
	break;
	case 2:
	{
		geom = std::shared_ptr<physx::PxGeometry>(new physx::PxBoxGeometry(reader->Read<physx::PxReal>(),
			reader->Read<physx::PxReal>(), reader->Read<physx::PxReal>()));
	}
	break;
	case 3:
	{
		convexPath = reader->ReadLongString();
		auto mesh = ContentManager::Load<physx::PxConvexMesh>(convexPath);
		geom = std::shared_ptr<physx::PxGeometry>(new physx::PxConvexMeshGeometry(mesh));
	}
	break;
	default:
	{
		Logger::Log(LogLevel::Error, L"GameObjectFactory Trying to create Colidercomponent with invalid Geomtype :"
			L"with type:" + colType);
		return nullptr;
	}
	}
	physx::PxTransform transform = reader->Read<physx::PxTransform	>();
	colComp = new ColliderComponent(geom, matPtr, transform);
	if (isConvex)
	{
		colComp->m_ConvexMeshPath = convexPath;
	}
	return colComp;
}
BaseComponent* ComponentFactory::CreateColliderComponent(const std::vector<PhysicsMaterialStruct*>& mats)
{
	physx::PxMaterial* matPtr = nullptr;

	matPtr = mats[0]->material;

	if (matPtr == nullptr)
	{
		Logger::Log(LogLevel::Error, L"GameObjectFactory Trying to create ColliderComponent default pxmat doesnt exist :"
			L"with materialname:" + mats[0]->materialName);
		return nullptr;
	}

	ColliderComponent* colComp = nullptr;
	//convex
	std::wstring convexPath;
	std::shared_ptr<physx::PxGeometry> geom;

	geom = std::shared_ptr<physx::PxGeometry>(new physx::PxBoxGeometry(1,1,1));
	
	
	physx::PxTransform transform = physx::PxTransform(0,0,0);
	colComp = new ColliderComponent(geom, mats[0], transform);
	
	return colComp;
}



BaseComponent* ComponentFactory::CreateFromFileSpriteComponent(BinaryReader* reader, std::vector<TextureStruct*>*  textures)
{
	std::wstring wname = reader->ReadString();
	std::string name(wname.begin(), wname.end());
	
	for(auto tex:*textures)
	{
		if(tex->name==name)
		{
			SpriteComponent* spriteComp = new SpriteComponent(tex, reader->Read<DirectX::XMFLOAT2>(),
				reader->Read<DirectX::XMFLOAT4>());
			return spriteComp;
		}
	}
	return nullptr;
}

BaseComponent* ComponentFactory::CreateBaseSpriteComponent(TextureStruct* baseText)
{

	SpriteComponent* spriteComp = new SpriteComponent( baseText);

		return spriteComp;
}

BaseComponent* ComponentFactory::CreateFromFileParticleEmmiterComponent(BinaryReader* reader,  std::vector<TextureStruct*>*  textures)
{

	std::wstring wname = reader->ReadString();
	std::string name(wname.begin(), wname.end());
	for (auto tex : *textures)
	{
		if (tex->name == name)
		{
			ParticleEmitterComponent* partEComp = new ParticleEmitterComponent(tex, reader->Read<int>());
			partEComp->SetVelocity(reader->Read<DirectX::XMFLOAT3>());
			partEComp->SetMinSize(reader->Read<float>());
			partEComp->SetMaxSize(reader->Read<float>());
			partEComp->SetMinSizeGrow(reader->Read<float>());
			partEComp->SetMaxSizeGrow(reader->Read<float>());
			partEComp->SetMinEmitterRange(reader->Read<float>());
			partEComp->SetMaxEmitterRange(reader->Read<float>());
			partEComp->SetColor(reader->Read<DirectX::XMFLOAT4>());
			return partEComp;
		}
	}


	return nullptr;
}

BaseComponent* ComponentFactory::CreateBaseParticleEmmiter(TextureStruct* baseText)
{
	ParticleEmitterComponent* partEComp = new ParticleEmitterComponent(baseText);
	return partEComp;
}

BaseComponent* ComponentFactory::CreateFromFileTerrainComponent(BinaryReader* reader)
{
	TerrainComponent* terComp = new TerrainComponent(reader->ReadLongString(), reader->ReadLongString(), reader->Read<unsigned int>(), reader->Read<unsigned int>(),
		reader->Read<float>(), reader->Read<float>(), reader->Read<float>());
	return terComp;
}


//model comps
BaseComponent* ComponentFactory::CreateFromFileModelComponent(BinaryReader* reader, std::vector<ModelReflectionStruct*>* models)
{
	std::wstring wModelname = reader->ReadString();
	std::string modelname = std::string(wModelname.begin(), wModelname.end());

	for (auto modelstr: *models)
	{
		if(modelstr->modelName==modelname)
		{
			ModelComponent* modComp = new ModelComponent(modelstr);
			modComp->SetMaterial(reader->Read<UINT>());
			return modComp;
		}
	}

	ModelComponent* modCompdefault = new ModelComponent(models->at(0));
	modCompdefault->SetMaterial(reader->Read<UINT>());
	return modCompdefault;

}
BaseComponent* ComponentFactory::CreateModelComponent(std::wstring path, UINT MaterialId)
{
	ModelComponent* modComp = new ModelComponent(path);
	modComp->SetMaterial(MaterialId);
	return modComp;
}
BaseComponent* ComponentFactory::CreateModelComponent(ModelReflectionStruct* str)
{
	ModelComponent* modComp = new ModelComponent(str);
	modComp->SetMaterial(0);
	return modComp;
}