#include "PreCompile.h"
#include "KnightSlash.h"
#include "KnightEffect.h"
#include <EngineBase/EngineRandom.h>
#include <EngineCore/EngineCore.h>
#include "FightUnit.h"

AKnightSlash::AKnightSlash()
{
	SetName("Knight Slash");

	float FrameTime = 0.015f;

	std::string SlashEffect = "SlashEffect";
	BodyRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 5, FrameTime, false);

	std::string UpSlashEffect = "UpSlashEffect";
	BodyRenderer->CreateAnimation(UpSlashEffect, UpSlashEffect, 0, 5, FrameTime, false);

	std::string DownSlashEffect = "DownSlashEffect";
	BodyRenderer->CreateAnimation(DownSlashEffect, DownSlashEffect, 0, 5, FrameTime, false);

	BodyRenderer->ChangeAnimation(SlashEffect);
}

AKnightSlash::~AKnightSlash()
{
}

void AKnightSlash::BeginPlay()
{
	AKnightSkill::BeginPlay();

}

void AKnightSlash::Tick(float _DeltaTime)
{
	AKnightSkill::Tick(_DeltaTime);
}

void AKnightSlash::CreateHitEffect(UCollision* _This, UCollision* _Other)
{
	UEngineDebug::OutPutString("SlashAttack");

	if (nullptr == Knight)
	{
		return;
	}

	AKnightEffect* Effect = GetWorld()->SpawnActor<AKnightEffect>().get();
	Effect->SetName("SlashAttack");
	Effect->SetZSort(EZOrder::KNIGHT_SKILL_SLASK_EFFECT);
	Effect->ChangeAnimation("NailHitEffect", Knight->GetActorLocation()); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
	Effect->SetScale(3.0f);
	Effect->GetRenderer()->SetMulColor({ 2.0f, 2.0f, 2.0f });
	AActor* Target = _Other->GetActor(); // Monster

	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector MonsterPos = { Target->GetActorLocation().X, Target->GetActorLocation().Y };
	FVector Direction = KnightPos - MonsterPos;

	Direction.Normalize();
	float Dir = Direction.Length();
	AMonster* Monster = dynamic_cast<AMonster*>(Target);
	if (nullptr == Monster)
	{
		return;
	}

	// 몬스터 위치에 따라서 이펙트가 출력되는 위치를 다르게 하고 싶다.
	// 몬스터 중앙에서 이펙트가 터지는게 아니라 몬스터와 나이트 위치를 비교하여 나이트의 공격과 충돌하는
	// 몬스터의 외곽부분에서 이펙트가 터진다.
	FVector Offset = Monster->GetRenderer()->GetRealScale().Half() * 0.5f;

	if (UEngineString::ToUpper("SlashEffect") == BodyRenderer->GetCurSpriteName())
	{
		Offset.Y = 0.0f;
	}
	else if (UEngineString::ToUpper("UpSlashEffect") == BodyRenderer->GetCurSpriteName())
	{
		Offset.Y *= -1.0f;
	}
	else if (UEngineString::ToUpper("DownSlashEffect") == BodyRenderer->GetCurSpriteName())
	{
		Offset.Y *= 1.0f;
		Offset.X = 0.0f;
		if (Monster->IsLeft())
		{
			Offset.Y *= -1.0f;
		}
	}
	if (true == Monster->IsFlip())
	{
		Offset.X *= -1.0f;
		Offset.Y *= -1.0f;
	}


	Effect->SetLocation(Target, Offset * Dir);

	UEngineRandom Random;
	float Degree = Random.Randomfloat(0.0f, 360.0f);
	FVector Rotation = { 0.0f, 0.0f, Degree };
	Effect->GetRenderer()->SetRotation(Rotation);
}

void AKnightSlash::Attack(UCollision* _This, UCollision* _Other)
{
	if (nullptr == Knight)
	{
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Other->GetActor());
	if (nullptr != Monster)
	{
		int KnightAtt = Knight->GetStatRef().GetAtt();
		UFightUnit::OnHit(Monster, KnightAtt);
		UFightUnit::RecoverMp(11);
		Monster->DamageLogic(KnightAtt);

		int MonsterHp = Monster->GetStatRef().GetHp();
		UEngineDebug::OutPutString("나이트가 몬스터에게 " + std::to_string(KnightAtt) + "만큼 데미지를 주었습니다. 현재 체력 : " + std::to_string(MonsterHp) );
		UEngineDebug::OutPutString("나이트가 마나를 획득하였습니다. 현재 마나 :  " + std::to_string(Knight->GetStatRef().GetMp()));

		Knockback(_This, _Other);
	}
}

void AKnightSlash::Knockback(UCollision* _This, UCollision* _Other)
{
	FVector TargetPos = { _Other->GetWorldLocation().X, _Other->GetWorldLocation().Y };
	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector KnockbackDirection = KnightPos - TargetPos;

	FVector MonsterKnockbackDirection = KnockbackDirection;
	if (UEngineString::ToUpper("SlashEffect") == BodyRenderer->GetCurSpriteName())
	{
		KnockbackDirection.Y = 0.0f;
		MonsterKnockbackDirection.Y = 0.0f;
	}
	else if (UEngineString::ToUpper("UpSlashEffect") == BodyRenderer->GetCurSpriteName())
	{
		KnockbackDirection = FVector::ZERO;
		MonsterKnockbackDirection.X = 0.0f;
	}
	else if (UEngineString::ToUpper("DownSlashEffect") == BodyRenderer->GetCurSpriteName())
	{
		KnockbackDirection.X = 0.0f;
		MonsterKnockbackDirection.X = 0.0f;

		float DeltaTime = UEngineCore::GetDeltaTime();
		TimeEventer->AddUpdateEvent(0.3f, [this, DeltaTime](float, float)
			{
				Knight->SetGravityForce(FVector::ZERO);
				Knight->AddActorLocation({ 0.0f, 700.0f * DeltaTime });
			});

	}

	KnockbackDirection.Normalize();
	MonsterKnockbackDirection.Normalize();

	Knight->GetStatRef().SetKnockbackDir(KnockbackDirection);

	AMonster* Monster = dynamic_cast<AMonster*>(_Other->GetActor());
	if (nullptr != Monster)
	{
		Monster->GetStatRef().SetKnockbackDir(-MonsterKnockbackDirection);
		Monster->GetStatRef().SetBeingHit(true);
	}
}


