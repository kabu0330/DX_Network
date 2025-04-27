#include "PreCompile.h"
#include "Vengefly.h"

AVengefly::AVengefly()
{
}

AVengefly::~AVengefly()
{
}

void AVengefly::BeginPlay()
{
	AMonster::BeginPlay();

	IdleSound = "mosquito_fly_loop.wav";
	StaticSound = "mosquito_fly_loop.wav";
	AttackSound = "FlyBug_Notice.wav";
}

void AVengefly::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AVengefly::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 180.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 3.0f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 15;
	Data.Hp = 8;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 300.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = 0.0f;
	InitJumpForce = 400.0f;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true
	bCanFly = true; // 날아다니는 몬스터라면 true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackFrame = 0.15f;// 매번 설정
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.1f;
}

void AVengefly::SetOffset()
{
	FVector ImageSize = { 603.0f , 799.0f };
	FVector SpriteSize = { 110.0f, 140.0f };

	DetectRange = { 1200, 1000 };
	CollisionScale = SpriteSize;

	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };

	DeathSpriteOffset = -30.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, 0.0f };
	AttackRendererOffset = { 0.0f, 0.0f };
	//DeathRotation = 1.0f;
}

void AVengefly::CreateAnimation()
{
	SetName("Vengefly");
	std::string MonsterStr = "Vengefly.png";
	float IdleTime = 0.2f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float DeathAirTime = 0.5f;
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 4, IdleTime);
	BodyRenderer->CreateAnimation("Walk", MonsterStr, 0, 4, RunnigTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", MonsterStr, 7, 10, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", MonsterStr, 11, 14, AttackFrame);
	BodyRenderer->CreateAnimation("AttackRecovery", MonsterStr, 0, 4, AttackRecoveryFrame);
	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 15, 16, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 17, 17, DeathTime, false);


	BodyRenderer->ChangeAnimation("Idle");
}
