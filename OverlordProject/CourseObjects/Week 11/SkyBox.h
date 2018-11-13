#pragma once
#include "GameObject.h"

class SkyBox : public GameObject
{
public:
	SkyBox(const std::wstring& cubeFileName);
	virtual ~SkyBox(void);

	virtual void Initialize(const GameContext& gameContext);

private:
 std::wstring m_CubeFileName;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBox(const SkyBox& t);
	SkyBox& operator=(const SkyBox& t);
};

