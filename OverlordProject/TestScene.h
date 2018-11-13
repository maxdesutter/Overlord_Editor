#pragma once
#include "GameScene.h"
#include "EffectHelper.h"

class Material;

class TestScene: public GameScene
{
public:
	TestScene(void);
	virtual ~TestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pTestObj1, *m_pTestObj2;
	float m_FpsInterval;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TestScene( const TestScene &obj);
	TestScene& operator=( const TestScene& obj);
};

