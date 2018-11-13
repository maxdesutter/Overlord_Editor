#include "stdafx.h"
#include "GUIManager.h"
#include "imgui.h"


bool GUIManager::IsMouseOverGUIWindow()
{
	ImGuiIO& io = ImGui::GetIO();


	return io.WantCaptureMouse;
}

bool GUIManager::IsWritingInTextBox()
{
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureKeyboard;
}

GUIManager::GUIManager()
{
}


GUIManager::~GUIManager()
{
}
