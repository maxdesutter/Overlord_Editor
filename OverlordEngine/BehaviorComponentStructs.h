#pragma once
#include <string>
struct BaseBehaviorComponentStruct
{
	virtual ~BaseBehaviorComponentStruct(){};
	std::string name;
	virtual void* Getnew() = 0;
};
template<typename  T>
struct BehaviorComponentStruct :public BaseBehaviorComponentStruct
{
	virtual ~BehaviorComponentStruct()override {} ;

	BehaviorComponentStruct()
	{
		name = typeid(T).name();
	}
	void* Getnew()
	{
		return new T();
	}
};