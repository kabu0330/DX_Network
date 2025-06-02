#include "PreCompile.h"
#include "GameField.h"
#include <EngineCore/Collision.h>

AGameField::AGameField()
{
	BodyRenderer->SetTexture("game_field.png", true, FConst::Half);
	BodyRenderer->SetZSort(FConst::MapZSort);


	SetActorLocation(FConst::FirstPlayerPos);
	FVector CurPos = GetActorLocation();

	Collision = CreateDefaultSubObject<UCollision>().get();
	Collision->SetupAttachment(RootComponent);
	Collision->SetScale3D(FVector(24, 24));
	Collision->SetCollisionProfileName("Debug");
	Collision->SetRelativeLocation({ (-FGlobals::MinoSize.X * 5 + FGlobals::MinoSize.X * 0.5f), (-FGlobals::MinoSize.Y * 10 + FGlobals::MinoSize.Y * 0.5f) });
	ReferencePoint = Collision->GetRelativeLocation();
	
	DebugCollision.resize(20);
	for (int i = 0; i < 20; i++)
	{
		DebugCollision[i] = CreateDefaultSubObject<UCollision>().get();
		DebugCollision[i]->SetupAttachment(RootComponent);
		DebugCollision[i]->SetScale3D(FVector(24, 24));
		DebugCollision[i]->SetCollisionProfileName("Debug");
		DebugCollision[i]->SetDebugColor(FVector(1.0f, 0.0f, 0.0f));
	}


	Point.resize(FConst::Point.Y);
	for (size_t i = 0; i < FConst::Point.Y; i++)
	{
		Point.resize(FConst::Point.X);
	}

	
}

AGameField::~AGameField()
{
}

FPoint AGameField::GetPoint(FVector _Pos)
{
	// X : 0 ~ 9, Y : 0 ~ 19
	int XPos = static_cast<int>(_Pos.X - ReferencePoint.X + FGlobals::MinoSize.X * 0.5f);
	int YPos = static_cast<int>(_Pos.Y - ReferencePoint.Y + FGlobals::MinoSize.Y * 0.5f);
	float X = ::abs(XPos) / (int)FGlobals::MinoSize.X;
	float Y = ::abs(YPos) / (int)FGlobals::MinoSize.Y;
	DebugCollision[0]->SetWorldLocation({ _Pos.X , _Pos.Y + 12 });
	return FPoint(X, Y);
}

void AGameField::BeginPlay()
{
	AServerActor::BeginPlay();
}

void AGameField::Tick(float _DeltaTime)
{
	AServerActor::Tick(_DeltaTime);

}

