#pragma once
#include "MeshFilter.h"

class ModelAnimator
{
public:
	ModelAnimator(MeshFilter* pMeshFilter);
	~ModelAnimator();


	void SetAnimation(std::wstring clipName);
	void SetAnimation(UINT clipNumber);
	void SetAnimation(AnimationClip clip);
	void Update(const GameContext& gameContext);
	void Reset(bool pause = true);
	void Play() { m_IsPlaying = true; }
	void Pause() { m_IsPlaying = false; }
	void SetPlayReversed(bool reverse) { m_Reversed = reverse; }
	void SetAnimationSpeed(float speedPercentage) { m_AnimationSpeed = speedPercentage; }

	bool IsPlaying() const { return m_IsPlaying; }
	bool IsReversed() const { return m_Reversed; }
	float GetAnimationSpeed() const { return m_AnimationSpeed; }
	UINT GetClipCount() const { return m_pMeshFilter->m_AnimationClips.size(); }
	std::wstring GetClipName() const { return m_ClipSet?m_CurrentClip.Name:L""; }
	std::vector<DirectX::XMFLOAT4X4> GetBoneTransforms() const { return m_Transforms; }
	std::vector<GetSetinterface*>* GetRegisteredMethods() { return  &m_GetSetMethods; }

private:
	void SetPlaying(bool Play) { m_IsPlaying = Play; };
	bool GetPlaying() { return m_IsPlaying; }

	UINT m_AnimationClipNumber = 0;
	void SetAnimationClipRef(UINT clip) ;
	UINT GetAnimationClipRef() { return m_AnimationClipNumber; }



	AnimationClip m_CurrentClip;
	MeshFilter* m_pMeshFilter;
 std::vector<DirectX::XMFLOAT4X4> m_Transforms;
	bool m_IsPlaying, m_Reversed, m_ClipSet;
	float m_TickCount, m_AnimationSpeed;



	template<class Type, typename OBJECT, typename GetterFunc, typename SetterFunc>
	void RegisterVariable(OBJECT obj, GetterFunc getterF, SetterFunc setterF, std::string name)
	{
		std::function<void(Type)> setter = std::bind(setterF, obj, std::placeholders::_1);
		std::function<Type()> getter = std::bind(getterF, obj);


		GetSetMethods<Type> *meths = new GetSetMethods<Type>;

		meths->varName = name;
		meths->Set = setter;
		meths->Get = getter;
		meths->type = GetReflectionType<Type>();

		m_GetSetMethods.push_back(meths);

	}
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ModelAnimator(const ModelAnimator& yRef);
	ModelAnimator& operator=(const ModelAnimator& yRef);



	std::vector<GetSetinterface*> m_GetSetMethods;
};

