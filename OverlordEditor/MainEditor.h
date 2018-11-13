#pragma once
#include "OverlordGame.h"

struct GameContext;
class MainEditor:public OverlordGame
{
public:
	MainEditor();
	~MainEditor();

protected:

	virtual void OnGamePreparing(GameSettings& gameSettings);
	virtual void Initialize();

};

