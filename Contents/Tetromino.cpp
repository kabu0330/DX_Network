#include "PreCompile.h"
#include "Tetromino.h"
#include <Engineplatform/EngineInput.h>
#include <EngineCore/NetHandler.h>

ATetromino::ATetromino()
{
	CurFramePacketTime = 20.0f * 30.0f;
	BodyRenderer->SetActive(false);

	CreateRenderers();	
	InitType();

}

void ATetromino::CreateRenderers()
{
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
			NewMinoRenderer->CreateAnimation("Block", "minos00.png", 0, 0);
			NewMinoRenderer->CreateAnimation("J_Mino", "minos00.png", 1, 1);
			NewMinoRenderer->CreateAnimation("S_Mino", "minos00.png", 2, 2);
			NewMinoRenderer->CreateAnimation("L_Mino", "minos00.png", 3, 3);
			NewMinoRenderer->CreateAnimation("T_Mino", "minos00.png", 4, 4);
			NewMinoRenderer->CreateAnimation("Z_Mino", "minos00.png", 5, 5);
			NewMinoRenderer->CreateAnimation("I_Mino", "minos00.png", 6, 6);
			NewMinoRenderer->CreateAnimation("O_Mino", "minos00.png", 7, 7);
			NewMinoRenderer->ChangeAnimation("S_Mino");
			Scale = NewMinoRenderer->GetCurScale();

			float XPos = (Scale.X * static_cast<float>(x)) - (Scale.X * 1.5f);
			float YPos = (Scale.Y * 1.0f) - (static_cast<float>(y) * Scale.Y);
			NewMinoRenderer->SetRelativeLocation({ XPos, YPos });
			FVector Pos = NewMinoRenderer->GetRelativeLocation();
			MinoRenders[y][x] = NewMinoRenderer;
		}
	}
}

void ATetromino::SetType(EMinoType _Type)
{
	MinoType = _Type;

	ChangeMino();
}

void ATetromino::InitType()
{
	for (size_t y = 0; y < MinoRenders.size(); y++)
	{
		for (size_t x = 0; x < MinoRenders[0].size(); x++)
		{
			MinoRenders[y][x]->SetActive(false);
		}
	}
}

void ATetromino::ChangeMino()
{
	InitType();

	switch (MinoType)
	{
	case EMinoType::I_MINO:
		for (size_t y = 0; y < MinoRenders.size(); y++)
		{
			MinoRenders[y][1]->ChangeAnimation("I_MINO");
			MinoRenders[y][1]->SetActive(true);
		}
		break;
	case EMinoType::O_MINO:
		for (int y = 1; y <= 2; y++)
		{
			for (int x = 1; x <= 2; x++)
			{
				MinoRenders[y][x]->ChangeAnimation("O_MINO");
				MinoRenders[y][x]->SetActive(true);

			}
		}
		break;
	case EMinoType::T_MINO:
		for (int y = 1; y <= 2; y++)
		{
			for (int x = 0; x <= 2; x++)
			{
				if ((2 == y && 0 == x) || (2 == y && 2 == x))
				{
					continue;
				}
				MinoRenders[y][x]->ChangeAnimation("T_MINO");
				MinoRenders[y][x]->SetActive(true);
			}
		}
		break;
	case EMinoType::J_MINO:
		for (int y = 0; y <= 2; y++)
		{
			for (int x = 1; x <= 2; x++)
			{
				if ((0 == y && 1 == x) || (1 == y && 1 == x))
				{
					continue;
				}
				MinoRenders[y][x]->ChangeAnimation("J_MINO");
				MinoRenders[y][x]->SetActive(true);
			}
		}
		break;
	case EMinoType::L_MINO:
		for (int y = 0; y <= 2; y++)
		{
			for (int x = 1; x <= 2; x++)
			{
				if ((0 == y && 2 == x) || (1 == y && 2 == x))
				{
					continue;
				}
				MinoRenders[y][x]->ChangeAnimation("L_MINO");
				MinoRenders[y][x]->SetActive(true);
			}
		}
		break;
	case EMinoType::S_MINO:
		for (int y = 1; y <= 2; y++)
		{
			for (int x = 0; x <= 2; x++)
			{
				if ((1 == y && 0 == x) || (2 == y && 2 == x))
				{
					continue;
				}
				MinoRenders[y][x]->ChangeAnimation("S_MINO");
				MinoRenders[y][x]->SetActive(true);
			}
		}
		break;
	case EMinoType::Z_MINO:
		for (int y = 1; y <= 2; y++)
		{
			for (int x = 0; x <= 2; x++)
			{
				if ((1 == y && 2 == x) || (2 == y && 0 == x))
				{
					continue;
				}
				MinoRenders[y][x]->ChangeAnimation("Z_MINO");
				MinoRenders[y][x]->SetActive(true);
			}
		}
		break;
	case EMinoType::MAX:
		break;
	default:
		break;
	}
}

void ATetromino::BeginPlay()
{
	AServerPawn::BeginPlay();

	SetType(MinoType);
}

void ATetromino::Tick(float _DeltaTime)
{
	AServerPawn::Tick(_DeltaTime);
}

void ATetromino::SetControllMode(float _DeltaTime)
{
	FVector Velocity = FVector::ZERO;
	FVector Speed(Scale.X, Scale.Y);

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
		AddActorRotation(FVector(0.0f, 0.0f, 90.0f));
		FVector Rotation = GetActorRotation();
	}
	if (true == UEngineInput::IsDown('E'))
	{
		int Type = static_cast<int>(MinoType);
		++Type;
		if (Type == static_cast<int>(EMinoType::MAX))
		{
			Type = 0;
		}
		SetType(Type);
	}

	AddActorWorldOffset(Velocity);

	auto SyncPosition = [this](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			_Packet->SetPosition(this->GetActorLocation());
			_Packet->SetRotation(this->GetActorRotation());
		};

	GetNetHandler()->SendPacket<UObjectUpdatePacket>(SyncPosition);

	auto SyncFunction = [this](std::shared_ptr<UMinoUpdatePacket> _Packet)
		{
			_Packet->SetMinoType(MinoType);
		};

	GetNetHandler()->SendPacket<UMinoUpdatePacket>(SyncFunction);

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

