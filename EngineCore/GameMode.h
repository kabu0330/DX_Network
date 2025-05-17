#pragma once
#include "Actor.h"

// ���� :
class AGameMode : public AActor
{
public:
	ENGINEAPI AGameMode();
	ENGINEAPI ~AGameMode();

protected:
	ENGINEAPI void BeginPlay() override; // ���Ӹ�� ���� ��� Test Pawn�� �����ϰ� ������ ȣ��

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

