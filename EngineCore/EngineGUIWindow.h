#pragma once
#include <EngineBase/Object.h>
#include <EngineCore/EngineGUI.h>

// Ό³Έν :
class UEngineGUIWindow : public UObject
{
	friend class UEngineGUI;

public:
	ENGINEAPI UEngineGUIWindow();
	ENGINEAPI ~UEngineGUIWindow();

	virtual void BeginPlay() {}
	virtual void OnGUI(float _DeltaTime) = 0;

	ULevel* GetWorld()
	{
		return World;
	}

protected:

private:
	ULevel* World;


private:
	// delete Function
	UEngineGUIWindow(const UEngineGUIWindow& _Other) = delete;
	UEngineGUIWindow(UEngineGUIWindow&& _Other) noexcept = delete;
	UEngineGUIWindow& operator=(const UEngineGUIWindow& _Other) = delete;
	UEngineGUIWindow& operator=(UEngineGUIWindow&& _Other) noexcept = delete;
};

