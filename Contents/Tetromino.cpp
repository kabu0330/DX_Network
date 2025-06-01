#include "PreCompile.h"
#include "Tetromino.h"
#include <Engineplatform/EngineInput.h>
#include <EngineCore/NetHandler.h>

ATetromino::ATetromino()
{
	CurFramePacketTime = 20.0f * 30.0f;
	BodyRenderer->CreateAnimation("Mino", "minos00.png", 0, 0);
	BodyRenderer->ChangeAnimation("Mino");
	BodyRenderer->SetActive(false);

	SetActorRelativeScale3D(FVector(1.0f, 1.0f));
	
	MinoRenders.resize(4);
	for (size_t i = 0; i < MinoRenders.size(); i++)
	{
		MinoRenders[i].resize(4);
	}

	for (size_t y = 0; y < MinoRenders.size(); y++)
	{
		for (size_t x = 0; x < MinoRenders[0].size(); x++)
		{
			USpriteRenderer* NewMinoRenderer = CreateDefaultSubObject<USpriteRenderer>().get();
			NewMinoRenderer->SetupAttachment(RootComponent);
			NewMinoRenderer->SetAutoScaleRatio(0.5f);
			NewMinoRenderer->CreateAnimation("Mino", "minos00.png", 0, 0);
			NewMinoRenderer->ChangeAnimation("Mino");
			FVector Scale = NewMinoRenderer->GetCurScale();

			float XPos = Scale.X * static_cast<float>(x) - Scale.X;
			float YPos = Scale.Y - (static_cast<float>(y) * Scale.Y);
			NewMinoRenderer->SetRelativeLocation({ XPos, YPos });
			FVector Pos = NewMinoRenderer->GetRelativeLocation();
			MinoRenders[y][x] = NewMinoRenderer;
		}
	}
	
}

ATetromino::~ATetromino()
{
}

void ATetromino::BeginPlay()
{
	AServerPawn::BeginPlay();
}

void ATetromino::Tick(float _DeltaTime)
{
	AServerPawn::Tick(_DeltaTime);
}

void ATetromino::SetControllMode(float _DeltaTime)
{
	FVector Velocity = FVector::ZERO;
	FVector Speed(100.0f, 100.0f);

	if (true == UEngineInput::IsDown('A') || UEngineInput::IsDown(VK_LEFT))
	{
		Velocity.X -= Speed.X;
	}
	if (true == UEngineInput::IsDown('D') || UEngineInput::IsDown(VK_RIGHT))
	{
		Velocity.X += Speed.X;
	}
	if (true == UEngineInput::IsDown('W') || UEngineInput::IsDown(VK_UP))
	{
		Velocity.Y += Speed.Y;
	}
	if (true == UEngineInput::IsDown('S') || UEngineInput::IsDown(VK_DOWN))
	{
		Velocity.Y -= Speed.Y;
	}
	if (true == UEngineInput::IsDown('Q'))
	{
		AddActorRotation(FVector(0.0f, 0.0f, 30.0f));
		FVector Rotation = GetActorRotation();
	}

	AddActorWorldOffset(Velocity);

	auto SyncPosition = [this](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			_Packet->SetPosition(this->GetActorLocation());
			_Packet->SetRotation(this->GetActorRotation());
		};

	GetNetHandler()->SendPacket<UObjectUpdatePacket>(SyncPosition);

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
}

