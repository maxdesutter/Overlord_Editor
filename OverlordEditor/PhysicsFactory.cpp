#include "OverlordEditorPCH.h"
#include "PhysicsFactory.h"
#include "BinaryReader.h"
#include "PhysxManager.h"

PhysicsFactory::PhysicsFactory()
{
}


PhysicsFactory::~PhysicsFactory()
{
}

PhysicsMaterialStruct* PhysicsFactory::CreatePhysicsMaterialFromFile(BinaryReader* reader, physx::PxPhysics* physx)
{
	std::wstring name = reader->ReadLongString();
	physx::PxMaterial* mat = physx->createMaterial(reader->Read<physx::PxReal>(), reader->Read<physx::PxReal>(), reader->Read<physx::PxReal>());
	PhysicsMaterialStruct* pmat = new PhysicsMaterialStruct(mat, name);

	return pmat;
}

