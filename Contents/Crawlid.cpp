#include "PreCompile.h"
#include "Crawlid.h"

ACrawlid::ACrawlid()
{
}

ACrawlid::~ACrawlid()
{
}

void ACrawlid::BeginPlay()
{
	AMonster::BeginPlay();

	StaticSound = "crawler.wav";
}

void ACrawlid::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void ACrawlid::SetStatus()
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
	Data.KnockbackDistance = 150.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = 0.0f;
	InitJumpForce = 400.0f;
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = false; // �����ϴ� ���͸� true
	bIsAggressive = false; // ȣ�����̸� true

	MoveDuration = 2.0f;
	MoveCooldown = 0.0f;

	AttackFrame = 0.15f;// �Ź� ����
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.1f;
}

void ACrawlid::SetOffset()
{
	FVector ImageSize = { 479.0f , 469.0f };
	FVector SpriteSize = { 120.0f, 82.0f };

	BodyCollisionOffset = { 0.0f, 0.0f }; // �ø��� ��ġ�� ���� �ȼ��浹 ��ġ�� �ٲ��.
	RendererOffset = { 0.0f, 0.0f };

	CollisionScale = SpriteSize;
	DetectRange = { 0, 0 };

	DeathSpriteOffset = -15.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, 0.0f };
	AttackRendererOffset = { 0.0f, 0.0f };
}

void ACrawlid::CreateAnimation()
{
	SetName("Crawlid");
	std::string Crawlid = "Crawlid.png";
	float IdleTime = 0.2f;
	float TurnTime = 0.3f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float DeathAirTime = 0.1f; // ���ư��� �ð� ������ ���� �ڿ��������� ǥ���ȴ�.
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", Crawlid, 0, 3, IdleTime);
	BodyRenderer->CreateAnimation("Walk", Crawlid, 0, 3, RunnigTime);
	BodyRenderer->CreateAnimation("DeathAir", Crawlid, 6, 8, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", Crawlid, 9, 10, DeathTime, false);

	BodyRenderer->ChangeAnimation("Idle");
}

void ACrawlid::DamageEvent()
{
	//Stat.SetVelocity(0.0f);
	//TimeEventer->AddEndEvent(0.3f, [this]()
	//	{
	//		Stat.SetVelocity(100.0f);
	//	});
}


