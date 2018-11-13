#include "OverlordEditorPCH.h"
#include "MovementScript.h"
#include "GameObject.h"
#include "TransformComponent.h"

MovementScript::MovementScript():BaseBehaviorComponent()
{
	m_ComponentClassName = typeid(*this).name();
	RegisterVariable<float>(this, &MovementScript::GetSpeed, &MovementScript::SetSpeed, "speed");
	//RegisterVariable<bool>(this, &MovementScript::Getbool, &MovementScript::Setbool, "bool");

}


MovementScript::~MovementScript()
{
}
void MovementScript::BehaviorInitialize(const GameContext& gameContext)
{
}

void MovementScript::BehaviorUpdate(const GameContext& gameContext)
{
	auto pos = GetGameObject()->GetTransform()->GetPosition();
	pos.x += m_Speed * gameContext.pGameTime->GetElapsed();

	GetGameObject()->GetTransform()->Translate(pos);
}

void MovementScript::BehaviorDraw(const GameContext& gameContext)
{
}


