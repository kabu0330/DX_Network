#include "PreCompile.h"
#include "Tetromino.h"

ATetromino::ATetromino()
{
	CurFramePacketTime = 20.0f * 30.0f;
	BodyRenderer->CreateAnimation("Mino", "minos00.png", 0, 0);
	BodyRenderer->ChangeAnimation("Mino");
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

