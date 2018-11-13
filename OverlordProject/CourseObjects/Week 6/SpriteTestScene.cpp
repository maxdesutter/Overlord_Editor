
#include "stdafx.h"

#include "SpriteTestScene.h"
#include "GameObject.h"


#include "SpriteRenderer.h"

#include "ContentManager.h"
#include "TextureData.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "MeshFilter.h"
#include "SpriteFont.h"
#include "TextRenderer.h"

#define FPS_COUNTER 1

SpriteTestScene::SpriteTestScene(void) :
GameScene(L"SpriteTestScene"),
m_FpsInterval(FPS_COUNTER),
m_pObj(nullptr),
m_Counter(105.0f)
{
}


SpriteTestScene::~SpriteTestScene(void)
{
}

void SpriteTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pTexture = ContentManager::Load<TextureData>(L"./Resources/Textures/Chair_Dark.dds");

	auto obj = new GameObject();
	obj->AddComponent(new SpriteComponent(L"./Resources/Textures/TestSprite.jpg", DirectX::XMFLOAT2(0.5f, 0.5f), DirectX::XMFLOAT4(1, 1, 1, 1.0f)));
	AddChild(obj);
	obj->GetTransform()->Translate(0.f, .0f, 1.f);
	//obj->GetTransform()->Rotate(0.f, 0.f, 45.f);

	//auto obj2 = new GameObject();
	//obj2->AddComponent(new SpriteComponent(L"./Resources/Textures/TestSprite.jpg", DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT4(1, 1, 1, 1.f)));
	//obj->AddChild(obj2);

	////m_pObj->GetTransform()->Translate(680.f, 360.f, 0.f);
	////m_pObj->GetTransform()->Rotate(0.f,0.f,45.f);
	//obj2->GetTransform()->Translate(0, 0, 1.f);
	//obj2->GetTransform()->Scale(0.5f, 0.5f, 1.f);

	//m_pObj = new GameObject();
	//m_pObj->AddComponent(new SpriteComponent(L"./Resources/Textures/TestSprite.jpg", DirectX::XMFLOAT2(0.6f, 0.6f), DirectX::XMFLOAT4(1, 1, 1, 1.f)));
	//obj2->AddChild(m_pObj);

	//m_pObj->GetTransform()->Translate(0, 0, 0.5f);
	//m_pObj->GetTransform()->Scale(1.0f, 1.0f, 1.f);

	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");
}

void SpriteTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//m_FpsInterval += gameContext.pGameTime->GetElapsed();
	//if (m_FpsInterval >= FPS_COUNTER)
	//{
	//	m_FpsInterval -= FPS_COUNTER;
	//	Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	//}

	//m_pObj->GetTransform()->Rotate(0, 0, XM_PIDIV2 * gameContext.pGameTime->GetTotal(), false);
	//SpriteRenderer::GetInstance()->Draw(m_pTexture, DirectX::XMFLOAT2(300, 300), (DirectX::XMFLOAT4) DirectX::Colors::Red, DirectX::XMFLOAT2(0.5f, 0.5f), XM_PIDIV2 * gameContext.pGameTime->GetTotal());
}

void SpriteTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
 std::wstringstream sFps;
	sFps << L"FPS: " << gameContext.pGameTime->GetFPS();
	//TextRenderer::GetInstance()->DrawText(m_pFont, sFps.str(), DirectX::XMFLOAT2(10, 35), (DirectX::XMFLOAT4)DirectX::Colors::Red);

	sFps.str(L"");
	sFps.clear();
	m_Counter -= gameContext.pGameTime->GetElapsed();
	sFps << (int) m_Counter;
	//sFps << "Counter " << (int) m_Counter;
	TextRenderer::GetInstance()->DrawText(m_pFont, sFps.str(), DirectX::XMFLOAT2(10, 10), (DirectX::XMFLOAT4) DirectX::Colors::Red);
}
