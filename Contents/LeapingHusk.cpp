#include "PreCompile.h"
#include "LeapingHusk.h"

ALeapingHusk::ALeapingHusk()
{
}

ALeapingHusk::~ALeapingHusk()
{
}

void ALeapingHusk::BeginPlay()
{
	AMonster::BeginPlay();
}

void ALeapingHusk::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void ALeapingHusk::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
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
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = false; // �����ϴ� ���͸� true
	bIsAggressive = true; // ȣ�����̸� true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackCooldown = 5.0f;
}

void ALeapingHusk::SetOffset()
{
	FVector ImageSize = { 2500.0f , 4000.0f};
	FVector SpriteSize = { 128.0f, 200.0f };
	
	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	//GravityPointOffset.Y = (ImageSize.Y - SpriteSize.Y) / 2.0f; // (�̹��� ũ�� - 1������ ũ��) / 2.0f
	WallPointOffest = { -((ImageSize.X / 2.0f) - SpriteSize.X * 1.5f), (ImageSize.Y  / 2.0f) - SpriteSize.Y * 2.4f}; // �̹������� �� ���� �ٸ���

	CollisionScale = { 100, 200 };
	DetectRange = { 700, 50 };

	DeathSpriteOffset = -70.0f;
	DeathAirDistance = 300.0f;
	//DeathRotation = 1.0f;
}

void ALeapingHusk::CreateAnimation()
{
	SetName("Monster");
	std::string ALeapingHusk = "LeapingHusk.png";
	float IdleTime = 0.2f;
	float TurnTime = 0.3f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float AttackTime = 0.08f;
	float RecoveryTime = 0.1f;
	float DeathAirTime = 0.5f;
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", ALeapingHusk, 0, 5, IdleTime);
	BodyRenderer->CreateAnimation("Walk", ALeapingHusk, 6, 12, RunnigTime);
	BodyRenderer->CreateAnimation("Turn", ALeapingHusk, 13, 14, TurnTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", ALeapingHusk, 15, 17, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", ALeapingHusk, 20, 25, AttackTime, false);
	BodyRenderer->CreateAnimation("AttackRecovery", ALeapingHusk, 26, 27, RecoveryTime, false);
	BodyRenderer->CreateAnimation("DeathAir", ALeapingHusk, 28, 28, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", ALeapingHusk, 29, 36, DeathTime, false);


	AttackDuration = AttackTime * 6.0f;
	BodyRenderer->ChangeAnimation("Idle");
}

void ALeapingHusk::CreateCollision()
{
	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D(CollisionScale);
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

void ALeapingHusk::SetAttack(float _DeltaTime)
{
	//UEngineDebug::OutPutString("Monster FSM : Attack");
	CheckDeath();
	ActiveGravity();

	Dash();
	Jump(_DeltaTime);

	CreateAttackLogicAndEffect();

	//   �ǰݽ�                        �˹��� ����Ǵ� ģ������ ��� ��ų ĵ��
	if (true == Stat.IsBeingHit() && true == Stat.IsKnockbackable())
	{
		bIsFirstIdle = true; // Idle�� ���ư��� �ݵ�� �־��ֱ�
		bIsShowEffect = false;
		FSM.ChangeState(EMonsterState::IDLE);
	}
	else
	{
		TimeEventer->AddEndEvent(AttackDuration, [this]()
			{
				bIsShowEffect = false; 
				FSM.ChangeState(EMonsterState::ATTACK_RECOVERY);
			});
	}
}

void ALeapingHusk::CreateAttackLogicAndEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}
	bIsShowEffect = true; 

	std::shared_ptr<AMonsterSkill> Skill = GetWorld()->SpawnActor<AMonsterSkill>();
	//SlashEffect->ChangeAnimation("SlashEffect");

	Skill->SetCollisionTime(AttackDuration);

	FVector CollisionScale = FVector(100, 100);
	Skill->SetCollisionScale(CollisionScale);
	FVector Offset = FVector{ -50.0f, 0.0f };
	if (true == bIsLeft)
	{
		Skill->SetLocation(this, Offset);
	}
	else
	{
		Skill->SetLocation(this, -Offset);
	}

}

