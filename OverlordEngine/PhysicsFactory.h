#pragma once
#include "ReflectionStructs.h"
class PhysxManager;
class BinaryReader;

class PhysicsFactory
{
public:
	PhysicsFactory();
	~PhysicsFactory();
	PhysicsMaterialStruct* CreatePhysicsMaterialFromFile(BinaryReader* reader, physx::PxPhysics* physx);
};

