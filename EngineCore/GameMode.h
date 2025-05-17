#pragma once
#include "Actor.h"

// 설명 :
class AGameMode : public AActor
{
public:
	ENGINEAPI AGameMode();
	ENGINEAPI ~AGameMode();

protected:
	ENGINEAPI void BeginPlay() override; // 게임모드 생성 즉시 Test Pawn을 생성하고 싶으면 호출

	class APlayerController* PlayerController = nullptr;

private:
	class ACameraActor* MainCamera = nullptr;


private:
	// delete Function
	AGameMode(const AGameMode& _Other) = delete;
	AGameMode(AGameMode&& _Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& _Other) = delete;
	AGameMode& operator=(AGameMode&& _Other) noexcept = delete;
};

