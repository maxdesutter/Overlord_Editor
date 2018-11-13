#pragma once
#include "BehaviorComponentStructs.h"


class GameObject;
class SceneManager;
class CameraComponent;
class PhysxProxy;
class PostProcessingMaterial;

class GameScene
{
public:
	GameScene(std::wstring sceneName);
	GameScene(const GameScene& other) = delete;
	GameScene(GameScene&& other) noexcept = delete;
	GameScene& operator=(const GameScene& other) = delete;
	GameScene& operator=(GameScene&& other) noexcept = delete;
	virtual ~GameScene();

	std::vector<ModelReflectionStruct*>* GetModels() { return &m_Models; }
	std::vector<PhysicsMaterialStruct*>* GetPhysxMaterials() { return &m_VecPhysicsMaterialsPtr; }
	std::vector<TextureStruct*>* GetTextures() { return &m_Textures; }
	std::vector<BaseBehaviorComponentStruct*>* GetScriptComponents() { return &m_ScriptComponents; }


	void AddChild(GameObject* obj);
	void RemoveChild(GameObject* obj, bool deleteObject = true);
	const GameContext& GetGameContext() const { return m_GameContext; }

	const std::vector<GameObject*>& GetChildren();

	PhysxProxy* GetPhysxProxy() const { return m_pPhysxProxy; }
	void SetActiveCamera(CameraComponent* pCameraComponent);

	// Update PP
	void AddPostProcessingEffect(PostProcessingMaterial* effect);
	void RemovePostProcessingEffect(PostProcessingMaterial* effect);

	std::wstring GetLevelName() { return m_LevelName; }
	void SetLevelName(std::wstring name) { m_LevelName = name; }

	bool GetPauseState() { return m_Pause; }
	void SetPauze(bool Paused);
protected:

	virtual void Initialize(const GameContext& gameContext) = 0;
	virtual void Update(const GameContext& gameContext) = 0;
	virtual void Draw(const GameContext& gameContext) = 0;

	virtual void SceneActivated() {}
	virtual void SceneDeactivated() {}

	// Update PP
	std::vector<PostProcessingMaterial*> m_PostProcessingEffects;

private:
	std::wstring m_LevelName = L"";

	friend class SceneManager;
	friend class EditorScene;

	void RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void RootUpdate();
	void RootDraw();
	void RootSceneActivated();
	void RootSceneDeactivated();
	void RootWindowStateChanged(int state, bool active) const;

	std::vector<GameObject*> m_pChildren;
	GameContext m_GameContext;
	bool m_IsInitialized;
	std::wstring m_SceneName;
	CameraComponent *m_pDefaultCamera, *m_pActiveCamera;

	// TODO: what if you have a game without physics? dependency should not exist here.
	PhysxProxy* m_pPhysxProxy;



	bool m_Pause;



	std::vector<PhysicsMaterialStruct*>m_VecPhysicsMaterialsPtr;
	std::vector<ModelReflectionStruct*>m_Models;
	std::vector<TextureStruct*>m_Textures;
	std::vector<BaseBehaviorComponentStruct*>m_ScriptComponents;
};
