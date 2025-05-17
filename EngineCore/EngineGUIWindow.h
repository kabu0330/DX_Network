#pragma once
#include <EngineBase/Object.h>

// Ό³Έν :
class UEngineGUIWindow : public UObject
{
	friend class UEngineGUI;

public:
	// constrcuter destructer
	ENGINEAPI UEngineGUIWindow();
	ENGINEAPI ~UEngineGUIWindow();

	// delete Function
	UEngineGUIWindow(const UEngineGUIWindow& _Other) = delete;
	UEngineGUIWindow(UEngineGUIWindow&& _Other) noexcept = delete;
	UEngineGUIWindow& operator=(const UEngineGUIWindow& _Other) = delete;
	UEngineGUIWindow& operator=(UEngineGUIWindow&& _Other) noexcept = delete;

	virtual void BeginPlay() {}
	virtual void OnGUI(float _DeltaTime) = 0;

	ULevel* GetWorld()
	{
		return World;
	}

protected:

private:
	ULevel* World;

};

