#pragma once
#include "TransformObject.h"
#include "EngineDataStruct.h"
#include "RenderUnit.h"

// 설명 :
class UWidget : public UObject, public UTransformObject
{
	friend AHUD;

public:
	// constrcuter destructer
	ENGINEAPI UWidget();
	ENGINEAPI ~UWidget();

	// delete Function
	UWidget(const UWidget& _Other) = delete;
	UWidget(UWidget&& _Other) noexcept = delete;
	UWidget& operator=(const UWidget& _Other) = delete;
	UWidget& operator=(UWidget&& _Other) noexcept = delete;

	virtual void Tick(float _DeltaTime);
	virtual void Render(class UEngineCamera* Camera, float _DeltaTime);

	class ULevel* GetWorld();


	void SetHoverEvent(std::function<void()> _Value)
	{
		Hover = _Value;
	}

	void SetDownEvent(std::function<void()> _Value)
	{
		Down = _Value;
	}

	void SetUpEvent(std::function<void()> _Value)
	{
		Up = _Value;
	}

private:
	std::function<void()> Hover;
	std::function<void()> Down;
	std::function<void()> Up;

	// 플레이스먼트 new 할때 사용할 녀석이므로 초기화를 하면 안된다.
	class AHUD* HUD;

};

