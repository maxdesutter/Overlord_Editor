#pragma once
#include "GameScene.h"
#include "EffectHelper.h"

class Material;

class UberScene: public GameScene
{
public:
	UberScene(void);
	virtual ~UberScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_FpsInterval;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	UberScene( const UberScene &obj);
	UberScene& operator=( const UberScene& obj);
};

