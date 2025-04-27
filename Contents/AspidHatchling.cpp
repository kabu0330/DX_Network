#include "PreCompile.h"
#include "AspidHatchling.h"

AAspidHatchling::AAspidHatchling()
{
}

AAspidHatchling::~AAspidHatchling()
{
}

void AAspidHatchling::BeginPlay()
{
	AMonster::BeginPlay();

	IdleSound = "hatchling_fly_loop_1.wav";
	StaticSound = "hatchling_fly_loop_1.wav";
	AttackSound = "";
	DeathSound = "hatchling_explode.wav";
}

void AAspidHatchling::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AAspidHatchling::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 1.0f;
	Data.DashSpeed = Data.Velocity * 1.0f;
	Data.MaxHp = 15;
	Data.Hp = 5;
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

	bIsDeathDestroy = true; // 죽으면 시체 없이 소멸할건지
}

void AAspidHatchling::SetOffset()
{
	FVector ImageSize = { 713.0f , 410.0f };
	FVector SpriteSize = { 80.0f, 80.0f };

	DetectRange = { 1200, 1200 };
	CollisionScale = SpriteSize + FVector(-20.0f, -20.0f);

	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };

	DeathSpriteOffset = -30.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, 0.0f };
	AttackRendererOffset = { 0.0f, 0.0f };
	//DeathRotation = 1.0f;
}

void AAspidHatchling::CreateAnimation()
{
	SetName("AspidHatchling");
	std::string MonsterStr = "AspidHatchling.png";
	float IdleTime = 0.2f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float DeathAirTime = 0.4f;
	float DeathTime = 0.18f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 3, IdleTime);
	BodyRenderer->CreateAnimation("Walk", MonsterStr, 0, 3, RunnigTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", MonsterStr, 0, 3, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", MonsterStr, 0, 3, AttackFrame);
	BodyRenderer->CreateAnimation("AttackRecovery", MonsterStr, 0, 3, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 6, 6, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 7, 10, DeathTime, false);


	BodyRenderer->ChangeAnimation("Idle");
}
