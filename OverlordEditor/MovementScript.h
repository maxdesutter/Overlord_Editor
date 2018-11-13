#pragma once
#include "BaseBehaviorComponent.h"
class MovementScript :public BaseBehaviorComponent
{
public:
	MovementScript();
	virtual ~MovementScript();

	void BehaviorUpdate(const GameContext& gameContext) override;
	void BehaviorDraw(const GameContext& gameContext) override;
	void BehaviorInitialize(const GameContext& gameContext) override;

	void SetSpeed(float speed) { m_Speed = speed; }
	float GetSpeed() { return m_Speed; }

	void Setbool(bool bools) { testbool = bools; }
	bool Getbool() { return testbool; }
private:
	float m_Speed = 1.f;
	bool testbool = false;
};

