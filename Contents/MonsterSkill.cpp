#include "PreCompile.h"
#include "MonsterSkill.h"
#include "FightUnit.h"
#include <EngineCore/EngineCore.h>

AMonsterSkill::AMonsterSkill()
{
	SetName("MonsterSkill");
	Collision->SetCollisionProfileName("MonsterObject");

	// 이펙트 없이 컬리전만
	std::string BlanckEffect = "BlanckEffect";
	BodyRenderer->CreateAnimation(BlanckEffect, "SlashEffect", 0, 0, CollisionDuration, false);

	float FrameTime = 0.04f;
	// 이펙트 포함
	std::string BossGroundWave = "BossGroundWave";
	BodyRenderer->CreateAnimation(BossGroundWave, BossGroundWave, 0, 15, FrameTime, false);

	std::string BossGroundWaveLoop = "BossGroundWaveLoop";
	BodyRenderer->CreateAnimation(BossGroundWaveLoop, BossGroundWave, 13, 15, FrameTime);

	BodyRenderer->ChangeAnimation("BlanckEffect");
}

AMonsterSkill::~AMonsterSkill()
{
}

void AMonsterSkill::BeginPlay()
{
	ASkill::BeginPlay();
	
}

void AMonsterSkill::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
}

void AMonsterSkill::SetCollisionEvent()
{
	Collision->SetCollisionEnter(std::bind(&AMonsterSkill::Collide, this, std::placeholders::_1, std::placeholders::_2));
}

void AMonsterSkill::Collide(UCollision* _This, UCollision* _Other)
{
	CreateHitEffect(_This, _Other);
	Attack(_This, _Other);
	bIsCollision = true;
}

void AMonsterSkill::CreateHitEffect(UCollision* _This, UCollision* _Other)
{
}

void AMonsterSkill::Attack(UCollision* _This, UCollision* _Other)
{
	if (nullptr == Knight)
	{
		return;
	}
	if (true == Knight->IsInvincible())
	{
		return;
	}

	int MonsterAtt = 1;
	UFightUnit::OnHit(Knight, MonsterAtt);
	Knight->SetInvicible(true);

	UEngineDebug::OutPutString("몬스터가 나이트에게 데미지를 주었습니다." );

	TimeEventer->AddEndEvent(0.3f, [this]()
		{
			Knockback();
		});
}

void AMonsterSkill::Knockback()
{
	FVector Pos = { GetActorLocation().X, GetActorLocation().Y };
	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector KnockbackDirection = KnightPos - Pos;
	KnockbackDirection.Y = 0.0f;
	KnockbackDirection.Normalize();
	KnockbackDirection += FVector::UP;
	KnockbackDirection.Normalize();

	if (nullptr != Monster)
	{
		if (true == Monster->IsFlip())
		{
			KnockbackDirection.X *= -1.0f;
		}
	}

	Knight->GetStatRef().SetKnockbackDistance(500.0f);
	Knight->GetStatRef().SetKnockbackDir(KnockbackDirection);
}

void AMonsterSkill::Release()
{
	float DeltaTime = UEngineCore::GetDeltaTime();

	TimeElapsed += DeltaTime;
	if (TimeElapsed >= CollisionDuration || true == bIsCollision || true == bIsPixelCollision)
	{
		TimeElapsed = 0.0f;
		BodyRenderer->Destroy();
		Collision->Destroy();
		Destroy();
	}
}

