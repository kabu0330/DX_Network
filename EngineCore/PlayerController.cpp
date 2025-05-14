#include "PreCompile.h"
#include "PlayerController.h"
#include <EnginePlatform/EngineInput.h>

APlayerController::APlayerController()
{
}

APlayerController::~APlayerController()
{
}

void APlayerController::Possess(AActor* _Pawn)
{
	if (nullptr != _Pawn)
	{
		Pawn = _Pawn;
		Pawn->SetOwner(this);
	}
}

void APlayerController::SwitchArrowKeys()
{
	if ('A' == ArrowKeys[0])
	{
		ArrowKeys[0] = VK_LEFT;
		ArrowKeys[1] = VK_RIGHT;
		ArrowKeys[2] = VK_UP;
		ArrowKeys[3] = VK_DOWN;
	}
	else
	{
		ArrowKeys[0] = 'A';
		ArrowKeys[1] = 'D';
		ArrowKeys[2] = 'W';
		ArrowKeys[3] = 'S';
	}
}

void APlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerController::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	HandleInput(_DeltaTime);
}

void APlayerController::HandleInput(float _DeltaTime)
{
	FVector Velocity = FVector::ZERO;

	if (true == UEngineInput::IsPress(ArrowKeys[0]))
	{
		Velocity.X -= Speed.X * _DeltaTime;
	}
	if (true == UEngineInput::IsPress(ArrowKeys[1]))
	{
		Velocity.X += Speed.X * _DeltaTime;
	}
	if (true == UEngineInput::IsPress(ArrowKeys[2]))
	{
		Velocity.Y += Speed.Y * _DeltaTime;
	}
	if (true == UEngineInput::IsPress(ArrowKeys[3]))
	{
		Velocity.Y -= Speed.Y * _DeltaTime;
	}

	Pawn->AddActorLocation(Velocity);
}
