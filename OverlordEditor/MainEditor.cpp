#include "OverlordEditorPCH.h"

#include "MainEditor.h"
#include "GeneralStructs.h"
#include "SceneManager.h"
#include "PhysxProxy.h"
#include "DebugRenderer.h"
#include "EditorScene.h"
#include "MaterialFactory.h"
MainEditor::MainEditor()
{
}


MainEditor::~MainEditor()
{
}

void MainEditor::OnGamePreparing(GameSettings& )
{

	//gameSettings.DirectX.EnableDeferredRendering = true;
}
void MainEditor::Initialize()
{
	SceneManager::GetInstance()->AddGameScene(new EditorScene());

}