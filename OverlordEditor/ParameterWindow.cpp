#include "OverlordEditorPCH.h"
#include "ParameterWindow.h"
#include "imgui.h"
#include "GameObject.h"
#include "GameScene.h"



#include "ObjectParameters.h"
#include "MaterialParameters.h"

ParameterWindow::ParameterWindow()
{
	m_ObjectParameters = new ObjectParameters();
	m_MaterialParameters = new MaterialParameters();
}
ParameterWindow::~ParameterWindow()
{
	delete m_ObjectParameters;
	delete m_MaterialParameters;
}

void ParameterWindow::Update(GameScene* scene, GameObject* Object,Material* material, PhysicsMaterialStruct* pxMat)
{
	//create window
	ImGui::SetNextWindowPos({ 1500,20 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 300,880 }, ImGuiCond_Once);
	ImGui::Begin("ParameterWindow", 0, { ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize });


	if (Object)
	{
		m_ObjectParameters->Update(scene, Object);
	}
	if(material)
	{
		m_MaterialParameters->UpdateMaterials(scene, material);
	}
	if(pxMat)
	{
		m_MaterialParameters->UpdatePhysxMaterials(scene, pxMat);
	}
	ImGui::End();
}
