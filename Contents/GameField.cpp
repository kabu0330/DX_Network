#include "PreCompile.h"
#include "GameField.h"

AGameField::AGameField()
{
	BodyRenderer->SetTexture("game_field.png", true, 0.5f);
}

AGameField::~AGameField()
{
}

void AGameField::BeginPlay()
{
	AServerActor::BeginPlay();
}

void AGameField::Tick(float _DeltaTime)
{
	AServerActor::Tick(_DeltaTime);

}

