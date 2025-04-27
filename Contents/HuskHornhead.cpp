#include "PreCompile.h"
#include "HuskHornhead.h"

AHuskHornhead::AHuskHornhead()
{
}

AHuskHornhead::~AHuskHornhead()
{
}

void AHuskHornhead::BeginPlay()
{
	AMonster::BeginPlay();
}

void AHuskHornhead::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AHuskHornhead::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 4.5f;
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

	JumpForce = 0.0f;
	InitJumpForce = 400.0f;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackFrame = 0.15f;// 매번 설정
	AttackDuration = AttackFrame * 12.0f; 
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.1f;
}

void AHuskHornhead::SetOffset()
{
	FVector ImageSize = { 2500.0f , 4000.0f };
	FVector SpriteSize = { 148.0f, 200.0f };

	RendererOffset = { 0.0f, -10.0f };
	BodyCollisionOffset = { 0.0f, -20.0f };

	CollisionScale = { 100, 150 };
	DetectRange = { 1100, 50 };

	DeathSpriteOffset = -50.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, -15.0f };
	AttackRendererOffset = { 0.0f, -40.0f };
	//DeathRotation = 1.0f;
}

void AHuskHornhead::CreateAnimation()
{
	SetName("HuskHornhead");
	std::string HuskHornhead = "HuskHornhead.png";
	float IdleTime = 0.2f;
	float TurnTime = 0.3f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float DeathAirTime = 0.5f;
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", HuskHornhead, 0, 5, IdleTime);
	BodyRenderer->CreateAnimation("Walk", HuskHornhead, 6, 12, RunnigTime);
	BodyRenderer->CreateAnimation("Turn", HuskHornhead, 13, 14, TurnTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", HuskHornhead, 15, 19, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", HuskHornhead, 20, 23, AttackFrame);
	BodyRenderer->CreateAnimation("AttackRecovery", HuskHornhead, 24, 24, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("DeathAir", HuskHornhead, 25, 25, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", HuskHornhead, 26, 33, DeathTime, false);


	BodyRenderer->ChangeAnimation("Idle");
}

void AHuskHornhead::CreateCollision()
{
	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D(CollisionScale);
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

void AHuskHornhead::CreateAttackLogicAndEffect()
{
	//if (true == bIsShowEffect)
	//{
	//	return;
	//}
	//bIsShowEffect = true;

	//std::shared_ptr<AMonsterSkill> Skill = GetWorld()->SpawnActor<AMonsterSkill>();
	//Skill->SetTargetActor(this);
	//Skill->SetCollisionTime(AttackDuration);

	//FVector CollisionScale = FVector(100, 50); 
	//Skill->SetCollisionScale(CollisionScale);
	//FVector Offset = FVector{ -50.0f, -25.0f };
	//if (true == bIsLeft)
	//{
	//	Skill->SetLocation(this, { Offset.X, Offset.Y });
	//}
	//else
	//{
	//	Skill->SetLocation(this, { -Offset.X, Offset.Y });
	//}
}

