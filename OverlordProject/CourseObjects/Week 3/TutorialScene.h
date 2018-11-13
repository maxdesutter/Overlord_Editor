#pragma once
#include "GameScene.h"

class TutorialScene : public GameScene
{
public:
	TutorialScene(void);
	virtual ~TutorialScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pSphere;
	physx::PxMaterial* m_PxMat;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TutorialScene( const TutorialScene &obj);
	TutorialScene& operator=( const TutorialScene& obj);
};


