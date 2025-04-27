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
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = false; // �����ϴ� ���͸� true
	bIsAggressive = false; // ȣ�����̸� true
	bCanFly = true;

	MoveDuration = 2.0f;
	MoveCooldown = 0.0f;

	AttackFrame = 0.15f;// �Ź� ����
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.1f;
}

void AGruzzer::SetOffset()
{
	FVector ImageSize = { 475.0f , 377.0f };
	FVector SpriteSize = { 82.0f, 106.0f };

	BodyCollisionOffset = { 0.0f, 0.0f }; // �ø��� ��ġ�� ���� �ȼ��浹 ��ġ�� �ٲ��.
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
	float DeathAirTime = 0.3f; // ���ư��� �ð� ������ ���� �ڿ��������� ǥ���ȴ�.
	float DeathTime = 0.12f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 3, IdleTime);
	BodyRenderer->CreateAnimation("Walk", MonsterStr, 0, 3, RunnigTime);
	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 4, 5, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 6, 9, DeathTime, false);

	BodyRenderer->ChangeAnimation("Idle");
}

