#pragma once
#include "Singleton.h"

class GUIManager:public Singleton<GUIManager>
{
public:


	GUIManager(const GUIManager& other) = delete;
	GUIManager(GUIManager&& other) noexcept = delete;
	GUIManager& operator=(const GUIManager& other) = delete;
	GUIManager& operator=(GUIManager&& other) noexcept = delete;


	bool IsMouseOverGUIWindow();
	bool IsWritingInTextBox();

private:
	friend class Singleton<GUIManager>;

	GUIManager();
	~GUIManager();
};

