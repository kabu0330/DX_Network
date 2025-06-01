#include "PreCompile.h"
#include "Tetromino.h"

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
}

