#pragma once
#include "GameScene.h"

class Character;

class PickSceneTest : public GameScene
{
public:
	PickSceneTest(void);
	virtual ~PickSceneTest(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	Character* m_pCharacter;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PickSceneTest( const PickSceneTest &obj);
	PickSceneTest& operator=( const PickSceneTest& obj);
};


