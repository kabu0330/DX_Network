#pragma once
#include "Actor.h"

// Ό³Έν :
class APlayerController : public AActor
{
public:
	ENGINEAPI APlayerController();
	ENGINEAPI ~APlayerController();

	ENGINEAPI void Possess(AActor* _Pawn);

	ENGINEAPI void SwitchArrowKeys();

	ENGINEAPI void SetSpeed(FVector _Speed)
	{
		Speed = _Speed;
	}
	ENGINEAPI FVector GetSpeed() const
	{
		return Speed;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	void HandleInput(float _DeltaTime);
	FVector Speed = FVector(100.0f, 100.0f, 100.0f);
	AActor* Pawn = nullptr;
	int ArrowKeys[4] = {'A', 'D', 'W', 'S'};

#define LEFT_KEY ArrowKeys[0]
#define RIGHT_KEY ArrowKeys[1]
#define UP_KEY ArrowKeys[2]
#define DOWN_KEY ArrowKeys[3]


private:
	// delete Function
	APlayerController(const APlayerController& _Other) = delete;
	APlayerController(APlayerController&& _Other) noexcept = delete;
	APlayerController& operator=(const APlayerController& _Other) = delete;
	APlayerController& operator=(APlayerController&& _Other) noexcept = delete;
};

