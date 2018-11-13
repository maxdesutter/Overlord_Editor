#pragma once

#include <string>
#include <functional>
#include "PxMaterial.h"
class ModelAnimator;
struct ModelReflectionStruct
{
	ModelReflectionStruct(std::string name , std::wstring path)
	{
		modelName = name;
		Path = path;
	}
	std::string modelName = "";
	std::wstring Path = L"";
};

struct ReflectionMaterialStruct
{
	ReflectionMaterialStruct(UINT id)
	{
		materialID = id;
	}
	UINT materialID;
};
struct PhysicsMaterialStruct
{
	physx::PxMaterial* material;
	std::wstring materialName;

	PhysicsMaterialStruct(physx::PxMaterial* mat, std::wstring name) :material(mat), materialName(name)
	{
		std::wstring* nm = &materialName;
		material->userData = nm;
	}

};
struct TextureStruct
{
	std::string name;
	std::wstring path;
	TextureStruct(std::wstring tpath, std::string tname) :name(tname), path(tpath)
	{

	}
};
enum ReflectionTypes
{
	TYPE_UNKNOWN,
	TYPE_INT,
	TYPE_UINT,
	TYPE_BOOL,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_MATERIAL,
	TYPE_MODEL,
	TYPE_XMFLOAT3,
	TYPE_XMFLOAT4,
	TYPE_PxGeometryType,
	TYPE_PXTRANSFORM,
	TYPE_PXMATERIAL,
	TYPE_TEXTURE,
	TYPE_ANIMATOR
};
template <class Type>
ReflectionTypes GetReflectionType()
{
	std::string tempName = typeid(Type).name();
	if (tempName == typeid(int).name())
		return TYPE_INT;
	if (tempName == typeid(UINT).name())
		return TYPE_UINT;
	if (tempName == typeid(float).name())
		return TYPE_FLOAT;
	if (tempName == typeid(bool).name())
		return TYPE_BOOL;
	if (tempName == typeid(DirectX::XMFLOAT3).name())
		return TYPE_XMFLOAT3;
	if (tempName == typeid(DirectX::XMFLOAT4).name())
		return TYPE_XMFLOAT4;
	if (tempName == typeid(std::string).name())
		return TYPE_STRING;
	if (tempName == typeid(ReflectionMaterialStruct).name())
		return TYPE_MATERIAL;
	if (tempName == typeid(ModelReflectionStruct*).name())
		return TYPE_MODEL;
	if (tempName == typeid(PhysicsMaterialStruct*).name())
		return TYPE_PXMATERIAL;
	if (tempName == typeid(physx::PxGeometryType::Enum).name())
		return TYPE_PxGeometryType;
	if (tempName == typeid(physx::PxTransform).name())
		return TYPE_PXTRANSFORM;
	if (tempName == typeid(TextureStruct*).name())
		return TYPE_TEXTURE;
	if (tempName == typeid(ModelAnimator*).name())
		return TYPE_ANIMATOR;
	return TYPE_UNKNOWN;
}


struct GetSetinterface
{
	GetSetinterface() { }
	virtual~GetSetinterface() {}

	//Type of the variable
	ReflectionTypes type;
};
template<class T>
struct GetSetMethods :public GetSetinterface
{
	GetSetMethods() {}
	virtual~GetSetMethods() {}

	//name of variable
	std::string varName;

	//Geter and setter
	std::function<T()>Get;
	std::function<void(T)>Set;
};