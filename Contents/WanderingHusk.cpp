#include "PreCompile.h"
#include "WanderingHusk.h"

AWanderingHusk::AWanderingHusk()
{
}

AWanderingHusk::~AWanderingHusk()
{
}

void AWanderingHusk::BeginPlay()
{
	AMonster::BeginPlay();
}

void AWanderingHusk::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AWanderingHusk::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 3.0f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 15;
	Data.Hp = 15;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 300.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = InitJumpForce;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true

	AttackCooldown = 5.0f;
	DeathAirDistance = 200.0f;
}

void AWanderingHusk::SetOffset()
{
	FVector ImageSize = { 479.0f , 469.0f };
	FVector SpriteSize = { 110.0f, 130.0f };

	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	CollisionScale = SpriteSize;
	//GravityPointOffset.Y = 1973.0f / 2.0f; // (이미지 크기 - 1프레임 크기) / 2.0f
	WallPointOffest = { -1394.0f / 2.0f, GravityPointOffset.Y - 130.0f };

	DetectRange = { 700, 50 };
}

void AWanderingHusk::CreateAnimation()
{
	SetName("Monster");
	std::string AWanderingHusk = "WanderingHusk.png";
	float IdleTime = 0.2f;
	float TurnTime = 0.3f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.15f;
	float AttackTime = 0.15f;
	float RecoveryTime = 1.5f;
	float DeathAirTime = 0.5f;
	float DeathTime = 0.15f;
	BodyRenderer->CreateAnimation("Idle", AWanderingHusk, 0, 5, IdleTime);
	BodyRenderer->CreateAnimation("Walk", AWanderingHusk, 6, 12, RunnigTime);
	BodyRenderer->CreateAnimation("Turn", AWanderingHusk, 13, 14, TurnTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", AWanderingHusk, 15, 19, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", AWanderingHusk, 20, 23, AttackTime);
	BodyRenderer->CreateAnimation("AttackRecovery", AWanderingHusk, 24, 24, RecoveryTime, false);
	BodyRenderer->CreateAnimation("DeathAir", AWanderingHusk, 25, 25, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", AWanderingHusk, 26, 33, DeathTime, false);

	AttackDuration = AttackTime * 4.0f;
	BodyRenderer->ChangeAnimation("Idle");
}

void AWanderingHusk::CreateCollision()
{
	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D({ 106, 127 });
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

