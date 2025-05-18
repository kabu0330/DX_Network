#include "PreCompile.h"
#include "ServerPawn.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/PlayerController.h>
#include "ServerGameMode.h"
#include "ContentPacket.h"

AServerPawn::AServerPawn()
{
	CurFramePacketTime = 20.0f * 30.0f;
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
	AServerGameMode* GameMode = GetWorld()->GetGameMode<AServerGameMode>();
	if (nullptr != GameMode->GetNetwork())
	{
		static float FrameCheck = 1.0f / 60.0f;
		CurFramePacketTime += _DeltaTime;
		if (CurFramePacketTime >= FrameCheck)
		{
			std::shared_ptr<UObjectUpdatePacket> Packet = CreatePacket<UObjectUpdatePacket>();
			Packet->SetPosition(GetActorLocation());
			GameMode->GetNetwork()->SendPacket(Packet);

			CurFramePacketTime -= _DeltaTime;
		}
	}
}

