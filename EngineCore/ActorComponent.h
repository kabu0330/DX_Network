#pragma once
#include <EngineBase/Object.h>

#include <memory>

// 설명 : 컴포넌트 최상위 클래스
class UActorComponent : public UObject
{
	friend class AActor;

public:
	ENGINEAPI UActorComponent();
	ENGINEAPI ~UActorComponent();

	template<typename ConvertType>
	ConvertType* GetActor()
	{
		return dynamic_cast<ConvertType*>(GetActor());
	}
	ENGINEAPI class AActor* GetActor();

	ENGINEAPI virtual void InitializeComponent() {}
	ENGINEAPI virtual void BeginPlay() {}
	ENGINEAPI virtual void ComponentTick(float _DeltaTime) {};

	ULevel* GetWorld();

	ENGINEAPI bool IsActive() override;

	ENGINEAPI bool IsDestroy() override;

protected:

private:
	// 액터가 CreateDefaultSubobject 함수를 호출하면 내부에서 호출한 액터 포인터 넣어준다.
	class AActor* Actor;


private:
	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;
};

