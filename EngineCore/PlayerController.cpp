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
		LEFT_KEY  = VK_LEFT;
		RIGHT_KEY = VK_RIGHT;
		UP_KEY    = VK_UP;
		DOWN_KEY  = VK_DOWN;
	}
	else
	{
		LEFT_KEY  = 'A';
		RIGHT_KEY = 'D';
		UP_KEY	  = 'W';
		DOWN_KEY  = 'S';
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

	if (true == UEngineInput::IsPress(LEFT_KEY))
	{
		Velocity.X -= Speed.X * _DeltaTime;
	}
	if (true == UEngineInput::IsPress(RIGHT_KEY))
	{
		Velocity.X += Speed.X * _DeltaTime;
	}
	if (true == UEngineInput::IsPress(UP_KEY))
	{
		Velocity.Y += Speed.Y * _DeltaTime;
	}
	if (true == UEngineInput::IsPress(DOWN_KEY))
	{
		Velocity.Y -= Speed.Y * _DeltaTime;
	}

	Pawn->AddActorLocation(Velocity);
}
