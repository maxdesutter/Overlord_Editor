#pragma once
#include "GameObject.h"

class ColliderComponent;

class Peddle : public GameObject
{
public:
	Peddle(float width, float height, float depth, DirectX::XMFLOAT4 color = (DirectX::XMFLOAT4)DirectX::Colors::Red);
	~Peddle(void);

	virtual void Initialize(const GameContext& gameContext);

private:

 DirectX::XMFLOAT3 m_Dimensions;
 DirectX::XMFLOAT4 m_Color;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Peddle(const Peddle& t);
	Peddle& operator=(const Peddle& t);
};

