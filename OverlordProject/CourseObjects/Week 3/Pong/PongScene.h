#pragma once
#include "GameScene.h"
#include "EffectHelper.h"

class Peddle;
class Wall;
class Ball;

class PongScene: public GameScene
{
public:
	PongScene(void);
	virtual ~PongScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	Peddle *m_pRightPeddle, *m_pLeftPeddle;
	Wall *m_pTopWall, *m_pBottomWall, *m_pLeftWall, *m_pRightWall;
	Ball *m_pBall;

	bool m_CanFire;

	static const int PEDDLE_SPEED = 350;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PongScene( const PongScene &obj);
	PongScene& operator=( const PongScene& obj);
};

