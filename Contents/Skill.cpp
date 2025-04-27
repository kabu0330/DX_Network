#include "PreCompile.h"
#include "Skill.h"
#include <EnginePlatform/EngineInput.h>
#include "FightUnit.h"

ASkill::ASkill()
{
	SetName("ASkill");

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	ZSort = static_cast<float>(EZOrder::KNIGHT_SKILL_FRONT);
	Collision->GetTransformRef().Location.Z = ZSort;
	Collision->SetCollisionProfileName("KnightObject");
	//Collision->SetCollisionType(ECollisionType::AABB);

	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });
}

void ASkill::BeginPlay()
{
	AEffect::BeginPlay();
	SetCollisionEvent();

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
	ActivePixelCollision();
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

void ASkill::SetCollisionEvent()
{
}

bool ASkill::IsCurRoom()
{
	if (nullptr == ParentRoom)
	{
		return false;
	}

	ARoom* CurRoom = ARoom::GetCurRoom();
	if (CurRoom == ParentRoom)
	{
		return true;
	}
	return false;
}

void ASkill::ActivePixelCollision()
{
	if (true == IsCurRoom())
	{
		FVector HalfScale = { CollisionScale.Half().X, -CollisionScale.Half().Y };
		if (true == bIsLeft)
		{
			HalfScale.X = -CollisionScale.Half().X;
		}

		FVector CollisionPoint = ParentRoom->GetPixelCollisionPoint(this, HalfScale);
		if (true == IsPixelCollision(CollisionPoint))
		{
			bIsPixelCollision = true; // 픽셀충돌
		}
		else
		{
			bIsPixelCollision = false;
		}
	}
}

bool ASkill::IsPixelCollision(FVector _CollisionPoint)
{
	FVector CollisionPoint = _CollisionPoint;
	CollisionPoint.RoundVector();

	UColor CollisionColor = ParentRoom->GetPixelCollisionImage().GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	if (CollisionColor != UColor::WHITE)
	{
		return true;
	}
	else
	{
		return false;
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

