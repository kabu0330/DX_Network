#include "PreCompile.h"
#include "MonsterProjectile.h"

AMonsterProjectile::AMonsterProjectile()
{
	float FrameTime = 0.1f;
	std::string Png = ".png";

	std::string BulletFire = "BulletFire";
	BodyRenderer->CreateAnimation(BulletFire, BulletFire + Png, 0, 6, FrameTime);

	std::string Fireball = "Fireball";
	BodyRenderer->CreateAnimation(Fireball, Fireball, 0, 3, FrameTime);

	std::string HitOrange = "HitOrange";
	std::string OrangeBullet = "OrangeBullet";
	BodyRenderer->CreateAnimation(OrangeBullet, HitOrange + Png, 0, 0, FrameTime);

	float RatationTime = 0.06f;
	std::string BossDropObject = "BossDropObject";
	BodyRenderer->CreateAnimation(BossDropObject, BossDropObject, 0, 7, RatationTime);

	std::string ShotImpact = "ShotImpact";
	BodyRenderer->CreateAnimation(ShotImpact, ShotImpact, 0, 5, RatationTime);


	BodyRenderer->ChangeAnimation(BulletFire);
}

AMonsterProjectile::~AMonsterProjectile()
{
}

void AMonsterProjectile::BeginPlay()
{
	AMonsterSkill::BeginPlay();
}

void AMonsterProjectile::Tick(float _DeltaTime)
{
	AMonsterSkill::Tick(_DeltaTime);
	ChangePixelCollisionAnimaion();
}

void AMonsterProjectile::Release()
{
	if (true == bIsCollision)
	{
		Collision->SetActive(false);
		BodyRenderer->SetActive(false);
	}

	TimeEventer->AddEndEvent(CollisionDuration, [this]()
		{
			Collision->Destroy();
			BodyRenderer->Destroy();
			Destroy();
		});
}

void AMonsterProjectile::ChangePixelCollisionAnimaion()
{
	if (true == bIsPixelCollision)
	{
		BodyRenderer->ChangeAnimation("ShotImpact");
		if (true == Collision->IsActive())
		{
			Sound = UEngineSound::Play("explosion_4_wet_original.wav");
			Sound.SetVolume(0.4f);
		}
		Collision->SetActive(false);
		if (true == BodyRenderer->IsCurAnimationEnd())
		{
			BodyRenderer->SetActive(false);
		}
	}
}

