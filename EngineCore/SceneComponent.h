#pragma once
#include <EngineBase/EngineMath.h>
#include "ActorComponent.h"
#include "TransformObject.h"

// 설명 :
class USceneComponent : public UActorComponent, public UTransformObject
{
	friend class AActor;

	// Local 부모영향 받음
	// World 안받음
	// 아무것도 없음 안받음

public:
	// constrcuter destructer
	USceneComponent();
	virtual ~USceneComponent() = 0;

	// delete Function
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	ENGINEAPI void ComponentTick(float _DeltaTime) override;

protected:

	ENGINEAPI void BeginPlay() override;


private:
};

