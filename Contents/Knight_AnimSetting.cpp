#include "PreCompile.h"
#include "Knight.h"

void AKnight::CreateRenderer()
{
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetName("Knight");
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = static_cast<float>(EZOrder::PLAYER);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	InitAnimation();
}

void AKnight::InitAnimation()
{
	float IdleFrameTime = 0.2f;
	float RunFrameTime = 0.1f;
	float ChangeFrameTime = 0.05f;
	float SlashFrameTime = 0.04f;

	// 이동 애니메이션
	std::string Idle = "Idle";
	BodyRenderer->CreateAnimation(Idle, Idle, 0, 8, IdleFrameTime);

	std::string Run = "Run";
	BodyRenderer->CreateAnimation(Run, Run, 0, 7, RunFrameTime);

	std::string RunToIdle = "RunToIdle";
	BodyRenderer->CreateAnimation(RunToIdle, RunToIdle, 0, 5, ChangeFrameTime, false);

	std::string IdleToRun = "IdleToRun";
	BodyRenderer->CreateAnimation(IdleToRun, IdleToRun, 0, 4, ChangeFrameTime, false);

	std::string Dash = "Dash";
	BodyRenderer->CreateAnimation(Dash, Dash, 0, 6, SlashFrameTime, false);

	std::string Jump = "Jump";
	BodyRenderer->CreateAnimation(Jump, Jump, 0, 7, ChangeFrameTime, false);

	std::string Airborn = "Airborn";
	BodyRenderer->CreateAnimation(Airborn, Airborn, 0, 2, RunFrameTime);

	std::string Land = "Land";
	BodyRenderer->CreateAnimation(Land, Land, 0, 2, RunFrameTime);

	std::string HardLand = "HardLand";
	BodyRenderer->CreateAnimation(HardLand, HardLand, 0, 9, RunFrameTime, false);


	// 정적 애니메이션
	float StaticFrameTime = 0.15f;
	std::string LookDown = "LookDown";
	BodyRenderer->CreateAnimation(LookDown, LookDown, 0, 5, StaticFrameTime, false);

	std::string LookDownLoop = "LookDownLoop";
	BodyRenderer->CreateAnimation(LookDownLoop, LookDownLoop, 0, 4, IdleFrameTime);

	std::string LookUp = "LookUp";
	BodyRenderer->CreateAnimation(LookUp, LookUp, 0, 5, StaticFrameTime, false);

	std::string LookUpLoop = "LookUpLoop";
	BodyRenderer->CreateAnimation(LookUpLoop, LookUpLoop, 0, 4, IdleFrameTime);


	// 전투 애니메이션
	//std::string Slash = "Slash";
	//BodyRenderer->CreateAnimation(Slash, Slash, 0, 6, SlashFrameTime, false);

	std::string SlashAlt = "SlashAlt";
	BodyRenderer->CreateAnimation(SlashAlt, SlashAlt, 0, 5, SlashFrameTime, false);

	std::string UpSlash = "UpSlash";
	BodyRenderer->CreateAnimation(UpSlash, UpSlash, 0, 6, SlashFrameTime, false);

	std::string DownSlash = "DownSlash";
	BodyRenderer->CreateAnimation(DownSlash, DownSlash, 0, 6, SlashFrameTime, false);


	// 스펠 애니메이션
	float FocusFrameTime = 0.05f;
	std::string Focus = "Focus";
	BodyRenderer->CreateAnimation(Focus, Focus, 0, 6, FocusFrameTime, false);

	std::string FocusGet = "FocusGet";
	BodyRenderer->CreateAnimation(FocusGet, FocusGet, 0, 10, FocusFrameTime, false);

	std::string FocusEnd = "FocusEnd";
	BodyRenderer->CreateAnimation(FocusEnd, FocusEnd, 0, 2, FocusFrameTime, false);

	std::string FireballAntic = "FireballAntic";
	BodyRenderer->CreateAnimation(FireballAntic, FireballAntic, 0, 2, ChangeFrameTime, false);

	float CastFrame = 0.07f;
	std::string FireballCast = "FireballCast";
	BodyRenderer->CreateAnimation(FireballCast, FireballCast, 0, 5, CastFrame, false);


	// 피격 애니메이션
	std::string Stun = "Stun";
	BodyRenderer->CreateAnimation(Stun, Stun, 0, 0, HitStunDuration, false);


	// 사망 애니메이션
	float DeathFrameTime = 0.2f;
	std::string DeathDamage = "DeathDamage";
	BodyRenderer->CreateAnimation(DeathDamage, DeathDamage, 0, 4, DeathFrameTime, false);

	std::string Death = "Death";
	BodyRenderer->CreateAnimation(Death, Death, 0, 12, RunFrameTime, false);

	std::string DeathHead = "DeathHead";
	BodyRenderer->CreateAnimation(DeathHead, DeathHead, 0, 1, 2.5f, false);
}

// 다음 애니메이션으로 변경
void AKnight::ChangeNextState(EKnightState _NextState)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(_NextState);
		return;
	}
}

// 이전 애니메이션으로 변경
void AKnight::ChangePrevState()
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(NextState);
		return;
	}
}