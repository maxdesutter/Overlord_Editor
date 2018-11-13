#pragma once
#include "BinaryWriter.h"
#include "ReflectionStructs.h"
class GameObject;
class TransformComponent;
struct GameContext;




class BaseComponent
{
	friend class GameObject;
	void RootInitialize(const GameContext& gameContext);
public:
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) noexcept = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) noexcept = delete;
	BaseComponent();
	virtual ~BaseComponent();

	GameObject* GetGameObject() const { return m_pGameObject; }
	TransformComponent* GetTransform() const;

	virtual void WriteToFile(BinaryWriter* writer) { writer->Write((unsigned char)0); }
	std::vector<GetSetinterface*>* GetRegisteredMethods() { return  &m_GetSetMethods; }
	virtual std::string GetComponentClassName() { return m_ComponentClassName; }
	void RunTimeInitialize(const GameContext& gameContext);
protected:

	virtual void Initialize(const GameContext& gameContext) = 0;
	virtual void Update(const GameContext& gameContext) = 0;
	virtual void Draw(const GameContext& gameContext) = 0;
	virtual void PostDraw(const GameContext&) {}

	GameObject* m_pGameObject;
	bool m_IsInitialized;

	std::string m_ComponentClassName = "BaseComponent";

	//variable registry
	template<class Type, typename OBJECT, typename GetterFunc, typename SetterFunc>
	void RegisterVariable(OBJECT obj, GetterFunc getterF, SetterFunc setterF, std::string name)
	{
		std::function<void(Type)> setter= std::bind(setterF, obj, std::placeholders::_1);
		std::function<Type()> getter = std::bind(getterF, obj);


		GetSetMethods<Type> *meths = new GetSetMethods<Type>;

		meths->varName = name;
		meths->Set = setter;
		meths->Get = getter;
		meths->type = GetReflectionType<Type>();

		m_GetSetMethods.push_back(meths);

	}
	template<class Type, typename OBJECT, typename GetterFunc>
	void RegisterGetter(OBJECT obj, GetterFunc getterF, std::string name)
	{
		std::function<Type()> getter = std::bind(getterF, obj);


		GetSetMethods<Type> *meths = new GetSetMethods<Type>;

		meths->varName = name;
		meths->Set = nullptr;
		meths->Get = getter;
		meths->type = GetReflectionType<Type>();

		m_GetSetMethods.push_back(meths);

	}
private:
	std::vector<GetSetinterface*> m_GetSetMethods;

};


