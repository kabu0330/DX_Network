#pragma once
#include "Actor.h"

// Ό³Έν :
class APlayerController : public AActor
{
public:
	// construter destructer
	ENGINEAPI APlayerController();
	ENGINEAPI ~APlayerController();

	// delete Function
	APlayerController(const APlayerController& _Other) = delete;
	APlayerController(APlayerController&& _Other) noexcept = delete;
	APlayerController& operator=(const APlayerController& _Other) = delete;
	APlayerController& operator=(APlayerController&& _Other) noexcept = delete;

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
};

