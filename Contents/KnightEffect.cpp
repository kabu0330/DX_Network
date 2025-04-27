#include "PreCompile.h"
#include "KnightEffect.h"

AKnightEffect::AKnightEffect()
{
	SetName("Knight Effect");

	float StunFrame = 0.09f;
	float FocusTime = 0.1f;
	float FrameTime = 0.06f;
	float BlastTime = 0.05f;
	float DashFrameTime = 0.03f;
	float DashFrameTime2 = 0.02f;

	SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FRONT));

	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 10, FocusTime, false);

	std::string BurstEffect = "BurstEffect";
	BodyRenderer->CreateAnimation(BurstEffect, BurstEffect, 0, 7, FrameTime, false);

	//std::string DashEffect = "Dash_effect";
	//BodyRenderer->CreateAnimation(DashEffect, DashEffect, 0, 5, DashFrameTime, false);

	std::string DashEffect2 = "Dash_effect2";
	BodyRenderer->CreateAnimation(DashEffect2, DashEffect2, 0, 7, DashFrameTime2, false);

	std::string NailHitEffect = "NailHitEffect";
	BodyRenderer->CreateAnimation(NailHitEffect, NailHitEffect, 0, 4, FrameTime, false);

	std::string Blast = "Blast";
	BodyRenderer->CreateAnimation(Blast, Blast, 0, 7, BlastTime, false);


	std::string StunEffect02 = "StunEffect02";
	BodyRenderer->CreateAnimation(StunEffect02, StunEffect02, 0, 6, StunFrame, false);


	BodyRenderer->ChangeAnimation(FocusEffect);
}

AKnightEffect::~AKnightEffect()
{
}

void AKnightEffect::BeginPlay()
{
	AEffect::BeginPlay();
}

void AKnightEffect::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);
}

