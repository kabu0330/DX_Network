#include "PreCompile.h"
#include "FalseKnightHead.h"
#include "FalseKnight.h"
#include "KnightSkill.h"
#include "FightUnit.h"
#include "KnightFireball.h"
#include <EngineBase/EngineRandom.h>

AFalseKnightHead::AFalseKnightHead()
{
	SetName("MonsterSkill");
	Collision->SetCollisionProfileName("MonsterHead");

	// ����Ʈ ���� �ø�����
	BodyRenderer->SetName("FalseKnight Head");
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	FVector HeadScale = { 120.0f, 120.0f };
	//BodyRenderer->SetAutoScale(false);
	//BodyRenderer->SetRelativeScale3D(HeadScale);
	ZSort = static_cast<float>(EZOrder::MONSTER) - 1.0f;
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	std::string AFalseKnightHead = "FalseKnightHead.png";
	std::string FalseKnightDeath = "FalseKnightDeath.png";

	float StunFrame = 0.5f;
	float FrameTime = 0.2f;
	float DeathTime = 0.2f;
	BodyRenderer->CreateAnimation("Stun", AFalseKnightHead, 0, 4, StunFrame);
	BodyRenderer->CreateAnimation("StunHit", AFalseKnightHead, 5, 7, FrameTime, false);
	BodyRenderer->CreateAnimation("Death", FalseKnightDeath, 0, 13, DeathTime, false);

	BodyRenderer->ChangeAnimation("Stun");
	BodyRenderer->SetActive(true);

	Collision->SetRelativeScale3D(HeadScale);
}

AFalseKnightHead::~AFalseKnightHead()
{
}

void AFalseKnightHead::BeginPlay()
{
	ASkill::BeginPlay();
}

void AFalseKnightHead::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
	
}

void AFalseKnightHead::SetCollisionEvent()
{
	Collision->SetCollisionEnter(std::bind(&AFalseKnightHead::Collide, this, std::placeholders::_1, std::placeholders::_2));
}

void AFalseKnightHead::Collide(UCollision* _This, UCollision* _Other)
{
	Attack(_This, _Other);
	CreateKnightHitEffect(_This, _Other);
}

void AFalseKnightHead::Attack(UCollision* _This, UCollision* _Other)
{
	UEngineDebug::OutPutString("��� ����!!!");

	if (nullptr == Boss)
	{
		MSGASSERT("������ ����� �����͸� ������ ���� �ʽ��ϴ�. SetParent �Լ��� �̿��Ͽ� �����Ͽ� �ּ���.");
		return;
	}
	
	int Att = Knight->GetStatRef().GetAtt();
	Boss->AddHeadHp(-Att);
	
	AActor* Actor = _Other->GetActor();
	AKnightFireball* Fireball = dynamic_cast<AKnightFireball*>(Actor);
	if (nullptr == Fireball)
	{
		UFightUnit::RecoverMp(11);
		UEngineDebug::OutPutString("����Ʈ�� ������ ȹ���Ͽ����ϴ�. ���� ���� :  " + std::to_string(Knight->GetStatRef().GetMp()));
	}
	else
	{
		UFightUnit::RecoverMp(-33);
		UEngineDebug::OutPutString("����Ʈ�� ������ �Һ��Ͽ����ϴ�. ���� ���� :  " + std::to_string(Knight->GetStatRef().GetMp()));
	}

	if (false == bIsDamage)
	{
		Boss->GetFSM().ChangeState(EMonsterState::STUN_HIT);
		bIsDamage = true;
	}
	UEngineRandom Random;
	int Result = Random.RandomInt(0, 1);
	if (0 == Result)
	{
		Sound = UEngineSound::Play("Fknight_hit_01.wav");
	}
	else
	{
		Sound = UEngineSound::Play("false_knight_head_damage_2.wav");
	}
}

void AFalseKnightHead::CreateKnightHitEffect(UCollision* _This, UCollision* _Other)
{
	AActor* Actor = _Other->GetActor();
	AKnightSkill* KnightSkill = dynamic_cast<AKnightSkill*>(Actor);
	if (nullptr == KnightSkill)
	{
		return;
	}
	KnightSkill->CreateHitEffect(_Other, Boss->GetCollision());
	KnightSkill->CreateWhiteHitParticleEffect(_Other, Boss->GetCollision());
	Boss->CreateOrangeParticleEffect();
}

void AFalseKnightHead::Release()
{
	if (false == bCanRelease)
	{
		return;
	}
	if (nullptr != Collision)
	{
		Collision->Destroy();
	}

	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		BodyRenderer->Destroy();
		Destroy();
	}
}

