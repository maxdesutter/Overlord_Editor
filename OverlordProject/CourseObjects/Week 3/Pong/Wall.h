#pragma once
#include "GameObject.h"

class ColliderComponent;

class Wall : public GameObject
{
public:
	Wall(float width, float height, float depth, bool isTrigger = false);
	virtual ~Wall();

	virtual void Initialize(const GameContext& gameContext);
	
	bool IsTriggered() const { return m_WasTriggered; }
	void Reset(){ m_WasTriggered = false; }

private:

 DirectX::XMFLOAT3 m_Dimensions;
	bool m_IsTrigger;
	bool m_WasTriggered;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Wall(const Wall& t);
	Wall& operator=(const Wall& t);
};

