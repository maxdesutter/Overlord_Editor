#pragma once
#include "GameScene.h"

class SpriteExComponent;

class HudTestScene : public GameScene
{
public:
	HudTestScene(void);
	virtual ~HudTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	SpriteExComponent* m_pCmpSprite = nullptr;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	HudTestScene( const HudTestScene &obj);
	HudTestScene& operator=( const HudTestScene& obj);
};


