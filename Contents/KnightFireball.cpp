#include "PreCompile.h"
#include "KnightFireball.h"
#include "FightUnit.h"
#include "KnightFireballEffect.h"

AKnightFireball::AKnightFireball()
{
	SetName("Fireball");
	float FireballFrame = 0.15f;
	std::string Fireball = "Fireball";
	BodyRenderer->CreateAnimation(Fireball, Fireball, 0, 3, FireballFrame);
	BodyRenderer->ChangeAnimation(Fireball);
}

AKnightFireball::~AKnightFireball()
{
}

void AKnightFireball::BeginPlay()
{
	AKnightSkill::BeginPlay();
}

void AKnightFireball::Tick(float _DeltaTime)
{
	AKnightSkill::Tick(_DeltaTime);
	if (true == bIsPixelCollision)
	{
		if (true == bIsEffect)
		{
			return;
		}
		bIsEffect = true;

		AKnightFireballEffect* Effect = GetWorld()->SpawnActor<AKnightFireballEffect>().get();
		Effect->SetName("FireballWallImpact");
		Effect->SetZSort(EZOrder::KNIGHT_SKILL_FIREBALL_EFFECT);
		AKnight* Knight = AKnight::GetPawn();
		Effect->ChangeAnimation("FireballWallImpact"); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
		Effect->SetScale(1.5f);
		Effect->ToggleFlip();
		FVector Offset = { 50.0f, 0.0f };
		if (nullptr != Collision && true == Collision->IsActive())
		{
			if (true == bIsLeft)
			{
				Offset *= -1.0f;
			}
			PointPos = Collision->GetWorldLocation() + Offset;
		}
		
		Effect->SetLocation(PointPos);
		Effect->GetRenderer()->SetMulColor({ 12.0f, 12.0f, 12.0f }, 0.1f);

		BodyRenderer->SetActive(false);
		Collision->SetActive(false);
	}
}

void AKnightFireball::CreateHitEffect(UCollision* _This, UCollision* _Other)
{
	UEngineDebug::OutPutString("Fireball Impact");
	AKnightFireballEffect* Effect = GetWorld()->SpawnActor<AKnightFireballEffect>().get();
	Effect->SetName("FireballImpact");
	Effect->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FIREBALL_EFFECT));
	AKnight* Knight = AKnight::GetPawn();
	//Effect->ChangeAnimation(Knight, "FireballImpact"); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
	Effect->ChangeAnimation("FireballImpact",Knight->GetActorLocation()); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
	Effect->SetScale(1.5f);
	AActor* Target = _Other->GetActor();
	Effect->SetLocation(Target);
}

void AKnightFireball::Attack(UCollision* _This, UCollision* _Other)
{
	if (nullptr == Knight)
	{
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Other->GetActor());
	if (nullptr != Monster)
	{
		int KnightAtt = Knight->GetStatRef().GetSpellAtt();
		UFightUnit::OnHit(Monster, KnightAtt);
		UFightUnit::RecoverMp(-33);
		Monster->DamageLogic(KnightAtt);

		int MonsterHp = Monster->GetStatRef().GetHp();
		UEngineDebug::OutPutString("나이트가 몬스터에게 " + std::to_string(KnightAtt) + "만큼 데미지를 주었습니다. 현재 체력 : " + std::to_string(MonsterHp));
		UEngineDebug::OutPutString("나이트가 마나를 소비하였습니다. 현재 마나 :  " + std::to_string(Knight->GetStatRef().GetMp()));

		Knockback(_This, _Other);
	}
}
