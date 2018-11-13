#pragma once
#include "GameScene.h"
#include "EffectHelper.h"

class MeshIndexedDrawComponent;

class ClothScene: public GameScene
{
public:
	ClothScene(void);
	virtual ~ClothScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pClothObject;
	MeshIndexedDrawComponent* m_pMeshDrawer;
	physx::PxCloth* m_pCloth;
	float m_FpsInterval;
	int m_ClothParticleCount;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ClothScene( const ClothScene &obj);
	ClothScene& operator=( const ClothScene& obj);
};

