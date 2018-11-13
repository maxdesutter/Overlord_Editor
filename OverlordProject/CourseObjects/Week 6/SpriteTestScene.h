#pragma once
#include "GameScene.h"

class TextureData;
class SpriteFont;

class SpriteTestScene : public GameScene
{
public:
	SpriteTestScene(void);
	virtual ~SpriteTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_FpsInterval;
	float m_Counter;
	TextureData* m_pTexture;
	GameObject* m_pObj;

	SpriteFont* m_pFont;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteTestScene(const SpriteTestScene &obj);
	SpriteTestScene& operator=(const SpriteTestScene& obj);
};

