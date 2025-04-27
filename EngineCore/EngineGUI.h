#pragma once

#include "imgui.h"
// impl단어를 보면 기존 기능에서 확장된 기능들을 의미하는 경우가 많다.
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// 설명 :
class UEngineGUI
{
public:
	// constrcuter destructer
	UEngineGUI();
	~UEngineGUI();

	// delete Function
	UEngineGUI(const UEngineGUI& _Other) = delete;
	UEngineGUI(UEngineGUI&& _Other) noexcept = delete;
	UEngineGUI& operator=(const UEngineGUI& _Other) = delete;
	UEngineGUI& operator=(UEngineGUI&& _Other) noexcept = delete;

	static void Init();
	static void Release();

	static void GUIRenderStart();

	static void GUIRenderEnd();

	ENGINEAPI static void PushGUIWindow(std::shared_ptr<class UEngineGUIWindow> _Window);

	template<typename WindowType>
	static std::shared_ptr<WindowType> CreateGUIWindow(std::string_view _Text)
	{
		std::string UpperName = UEngineString::ToUpper(_Text);

		if (nullptr != FindGUIWindow(UpperName))
		{
			MSGASSERT("이미 만들어진 윈도우를 또 만들수는 없습니다.");
			return nullptr;
		}

		std::shared_ptr<WindowType> Window = std::make_shared<WindowType>();
		Window->SetName(UpperName);
		PushGUIWindow(Window);
		return Window;
	}

	template<typename WindowType>
	static std::shared_ptr<WindowType> FindGUIWindow(std::string_view _Text)
	{
		return std::dynamic_pointer_cast<WindowType>(FindGUIWindow(_Text));
	}

	ENGINEAPI static std::shared_ptr<UEngineGUIWindow> FindGUIWindow(std::string_view _Text);

	static void GUIRender(ULevel* _Level);

	ENGINEAPI static void AllWindowOff();

	ENGINEAPI static void AllWindowOn();

protected:

private:
	static inline std::map<std::string, std::shared_ptr<class UEngineGUIWindow>> Windows;

};

