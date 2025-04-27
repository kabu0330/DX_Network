#include "PreCompile.h"
#include "MonsterEffect.h"

AMonsterEffect::AMonsterEffect()
{
	SetName("Monster Effect");

	float DebugFrame = 1.0f;
	float ImpactTime = 0.1f;
	float StunFrame = 0.09f;
	float FocusTime = 0.1f;
	float FrameTime = 0.06f;
	float BlastTime = 0.05f;
	float DashFrameTime = 0.03f;
	float DashFrameTime2 = 0.02f;

	SetZSort(static_cast<int>(EZOrder::MONSTER_SKILL_FRONT));

	std::string Png = ".png";
	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 10, FocusTime, false);

	std::string BurstEffect = "BurstEffect";
	BodyRenderer->CreateAnimation(BurstEffect, BurstEffect, 0, 7, FrameTime, false);

	std::string GroundImapctEffect = "GroundImapctEffect";
	BodyRenderer->CreateAnimation(GroundImapctEffect, GroundImapctEffect + Png, 0, 3, ImpactTime, false);


	BodyRenderer->ChangeAnimation(FocusEffect);
}

AMonsterEffect::~AMonsterEffect()
{
}

void AMonsterEffect::BeginPlay()
{
	AEffect::BeginPlay();
}

void AMonsterEffect::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);
}

