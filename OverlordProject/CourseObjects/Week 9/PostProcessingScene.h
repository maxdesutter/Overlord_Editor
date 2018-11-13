#pragma once
#include "GameScene.h"

class ModelComponent;
class SpriteFont;

class PostProcessingScene : public GameScene
{
public:
	PostProcessingScene(void);
	virtual ~PostProcessingScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	ModelComponent* m_pModel;
	SpriteFont* m_pFont;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostProcessingScene(const PostProcessingScene &obj);
	PostProcessingScene& operator=(const PostProcessingScene& obj);
};

