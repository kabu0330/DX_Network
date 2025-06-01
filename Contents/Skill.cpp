#include "PreCompile.h"
#include "Skill.h"
#include <EnginePlatform/EngineInput.h>

ASkill::ASkill()
{
	SetName("ASkill");

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	ZSort = static_cast<float>(EZOrder::KNIGHT_SKILL_FRONT);
	Collision->GetTransformRef().Location.Z = ZSort;
	Collision->SetCollisionProfileName("KnightObject");
	//Collision->SetCollisionType(ECollisionType::AABB);

	BodyRenderer->SetActorLocation({ 0.0f, 0.0f, ZSort });
}

void ASkill::BeginPlay()
{
	AEffect::BeginPlay();

	if (nullptr != BodyRenderer)
	{
		if (FVector::ZERO == CollisionScale)
		{
			CollisionScale = BodyRenderer->GetScale();
			Collision->SetScale3D(CollisionScale);
		}
	}
	Collision->SetScale3D(CollisionScale);
}

void ASkill::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);

	ChangeNextAnimation();
	Release();
}

void ASkill::Release()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}

	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		if (nullptr != Collision)
		{
			Collision->Destroy();
		}
		Destroy();
	}
}

void ASkill::ChangeNextAnimation()
{
	if (true == bIsNextAnimation)
	{
		if (true == BodyRenderer->IsCurAnimationEnd())
		{
			BodyRenderer->ChangeAnimation(NextAnimationName);
			bIsNextAnimation = false;
		}
	}
}

ASkill::~ASkill()
{
}

