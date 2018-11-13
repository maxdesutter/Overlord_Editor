#pragma once
#include "GameScene.h"

class TerrainTestScene : public GameScene
{
public:
	TerrainTestScene(void);
	virtual ~TerrainTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TerrainTestScene(const TerrainTestScene &obj);
	TerrainTestScene& operator=(const TerrainTestScene& obj);
};

