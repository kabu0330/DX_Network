#include "PreCompile.h"
#include "KnightFireballEffect.h"

AKnightFireballEffect::AKnightFireballEffect()
{
	float FrameTime = 0.06f;

	std::string FireballImpact = "FireballImpact";
	BodyRenderer->CreateAnimation(FireballImpact, FireballImpact, 0, 5, FrameTime, false);

	std::string FireballWallImpact = "FireballWallImpact";
	BodyRenderer->CreateAnimation(FireballWallImpact, FireballWallImpact, 0, 6, FrameTime, false);

	BodyRenderer->ChangeAnimation(FireballImpact);
}

AKnightFireballEffect::~AKnightFireballEffect()
{
}

void AKnightFireballEffect::BeginPlay()
{
	AKnightEffect::BeginPlay();
}

void AKnightFireballEffect::Tick(float _DeltaTime)
{
	AKnightEffect::Tick(_DeltaTime);
}

