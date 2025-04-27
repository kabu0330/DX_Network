#include "PreCompile.h"
#include "Gruzzer.h"

AGruzzer::AGruzzer()
{
}

AGruzzer::~AGruzzer()
{
}

void AGruzzer::BeginPlay()
{
	AMonster::BeginPlay();
	IdleSound = "fly_flying.wav";
	StaticSound = "fly_flying.wav";
	DeathSound = "hatchling_explode.wav";
}

void AGruzzer::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AGruzzer::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 100.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
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
	bIsAggressive = false; // 호전적이면 true
	bCanFly = true;

	MoveDuration = 2.0f;
	MoveCooldown = 0.0f;

	AttackFrame = 0.15f;// 매번 설정
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.1f;
}

void AGruzzer::SetOffset()
{
	FVector ImageSize = { 475.0f , 377.0f };
	FVector SpriteSize = { 82.0f, 106.0f };

	BodyCollisionOffset = { 0.0f, 0.0f }; // 컬리전 위치에 따라 픽셀충돌 위치도 바뀐다.
	RendererOffset = { 0.0f, 0.0f };

	CollisionScale = SpriteSize;
	DetectRange = { 0, 0 };

	DeathSpriteOffset = -53.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, 0.0f };
	AttackRendererOffset = { 0.0f, 0.0f };
}

void AGruzzer::CreateAnimation()
{
	SetName("Gruzzer");
	std::string MonsterStr = "Gruzzer.png";
	float IdleTime = 0.2f;
	float TurnTime = 0.3f;
	float RunnigTime = 0.1f;
	float DeathAirTime = 0.3f; // 날아가는 시간 조절에 따라서 자연스러움이 표현된다.
	float DeathTime = 0.12f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 3, IdleTime);
	BodyRenderer->CreateAnimation("Walk", MonsterStr, 0, 3, RunnigTime);
	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 4, 5, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 6, 9, DeathTime, false);

	BodyRenderer->ChangeAnimation("Idle");
}

