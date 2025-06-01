#include "PreCompile.h"
#include "ServerPawn.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/PlayerController.h>
#include "ServerGameMode.h"
#include "ContentPacket.h"
#include <EnginePlatform/EngineInput.h>

AServerPawn::AServerPawn()
{
	InitActorData(this);

	CurFramePacketTime = 20.0f * 30.0f;
	BodyRenderer->CreateAnimation("Mino", "minos00.png", 0, 0);
	BodyRenderer->ChangeAnimation("Mino");
}

AServerPawn::~AServerPawn()
{
}

void AServerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AServerPawn::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	if (true == IsControll())
	{
		SetControllMode(_DeltaTime);
	}
	else
	{
		SetNetMode(_DeltaTime);
	}
}

void AServerPawn::SetNetMode(float _DeltaTime)
{
}

void AServerPawn::SetControllMode(float _DeltaTime)
{
	FVector Velocity = FVector::ZERO;
	FVector Speed(100.0f, 100.0f);

	if (true == UEngineInput::IsPress('A'))
	{
		Velocity.X -= Speed.X * _DeltaTime;
	}
	if (true == UEngineInput::IsPress('D'))
	{
		Velocity.X += Speed.X * _DeltaTime;
	}
	if (true == UEngineInput::IsPress('W'))
	{
		Velocity.Y += Speed.Y * _DeltaTime;
	}
	if (true == UEngineInput::IsPress('S'))
	{
		Velocity.Y -= Speed.Y * _DeltaTime;
	}

	AddActorWorldOffset(Velocity);

	//AServerGameMode* GameMode = GetWorld()->GetGameMode<AServerGameMode>();
	//if (nullptr != GameMode->GetNetwork())
	//{
	//	static float FrameCheck = 1.0f / 60.0f;
	//	CurFramePacketTime += _DeltaTime;
	//	if (CurFramePacketTime >= FrameCheck)
	//	{
	//		std::shared_ptr<UObjectUpdatePacket> Packet = CreatePacket<UObjectUpdatePacket>();
	//		Packet->SetPosition(GetActorLocation());
	//		GameMode->GetNetwork()->SendPacket(Packet);

	//		CurFramePacketTime -= _DeltaTime;
	//	}
	//}
	
	auto SyncPosition = [this](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			_Packet->SetPosition(this->GetActorLocation());
		};

	GetNetHandler()->SendPacket<UObjectUpdatePacket>(SyncPosition);
}

