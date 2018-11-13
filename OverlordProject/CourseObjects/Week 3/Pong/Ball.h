#pragma once
#include "GameObject.h"

class ColliderComponent;

class Ball : public GameObject
{
public:
	Ball(float size, DirectX::XMFLOAT4 color = (DirectX::XMFLOAT4)DirectX::Colors::Red);
	~Ball(void);

	virtual void Initialize(const GameContext& gameContext);

private:

	float m_Size;
 DirectX::XMFLOAT4 m_Color;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Ball(const Ball& t);
	Ball& operator=(const Ball& t);
};

