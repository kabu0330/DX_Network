#pragma once
#include "SceneComponent.h"

// Ό³Έν :
class UDefaultSceneComponent : public USceneComponent
{
public:
	// constrcuter destructer
	ENGINEAPI UDefaultSceneComponent();
	ENGINEAPI ~UDefaultSceneComponent();

	// delete Function
	UDefaultSceneComponent(const UDefaultSceneComponent& _Other) = delete;
	UDefaultSceneComponent(UDefaultSceneComponent&& _Other) noexcept = delete;
	UDefaultSceneComponent& operator=(const UDefaultSceneComponent& _Other) = delete;
	UDefaultSceneComponent& operator=(UDefaultSceneComponent&& _Other) noexcept = delete;

protected:

private:

};

