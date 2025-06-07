#pragma once
#include <EngineCore/EngineGUIWindow.h>

// Ό³Έν :
class UDebugWindowGUI : public UEngineGUIWindow
{
public:
	UDebugWindowGUI();
	~UDebugWindowGUI();

	void GetMousePos();
	void GetFrame();


protected:
	void OnGUI(float _DelataTime) override;

private:

private:
	// delete Function
	UDebugWindowGUI(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI(UDebugWindowGUI&& _Other) noexcept = delete;
	UDebugWindowGUI& operator=(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI& operator=(UDebugWindowGUI&& _Other) noexcept = delete;
};

