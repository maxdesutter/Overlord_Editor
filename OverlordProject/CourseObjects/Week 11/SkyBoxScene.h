#pragma once
#include "GameScene.h"

class ParticleEmitterComponent;
class ModelComponent;

class SkyBoxScene : public GameScene
{
public:
	SkyBoxScene(void);
	virtual ~SkyBoxScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	ParticleEmitterComponent* m_pParticleEmitter;
	ModelComponent* m_pModel;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxScene(const SkyBoxScene &obj);
	SkyBoxScene& operator=(const SkyBoxScene& obj);
};

