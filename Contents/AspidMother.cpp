#include "PreCompile.h"
#include "AspidMother.h"
#include "AspidHatchling.h"

AAspidMother::AAspidMother()
{
}

AAspidMother::~AAspidMother()
{
}

void AAspidMother::BeginPlay()
{
	AMonster::BeginPlay();
	ChildZSort = GetActorLocation().Z;

	IdleSound = "hatcher_fly_loop.wav";
	StaticSound = "hatcher_fly_loop.wav";
	AttackSound = "";
	DeathSound = "hatchling_explode.wav";
}

void AAspidMother::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AAspidMother::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 80.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 1.0f;
	Data.DashSpeed = Data.Velocity * 0.0f; // ���� �߿� �ȿ����̷���
	Data.MaxHp = 20;
	Data.Hp = 20;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 400.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = 0.0f;
	InitJumpForce = 400.0f;
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = false; // �����ϴ� ���͸� true
	bIsAggressive = true; // ȣ�����̸� true
	bCanFly = true; // ���ƴٴϴ� ���Ͷ�� true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackFrame = 0.15f;// �Ź� ����
	AttackDuration = AttackFrame * 3.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.01f;

	bIsDeathDestroy = true; // ������ ��ü ���� �Ҹ��Ұ���

}

void AAspidMother::SetOffset()
{
	FVector ImageSize = { 787.0f , 1813.0f };
	FVector SpriteSize = { 160.0f, 180.0f };

	DetectRange = { 1100, 1500 };
	CollisionScale = SpriteSize;

	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };

	DeathSpriteOffset = -30.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, 0.0f };
	AttackRendererOffset = { 0.0f, 0.0f };
	//DeathRotation = 1.0f;
}

void AAspidMother::CreateAnimation()
{
	SetName("AspidMother");
	std::string MonsterStr = "AspidMother.png";
	float IdleTime = 0.2f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.15f;
	float DeathAirTime = 0.5f; // ���ư��� �ð� ������ ���� �ڿ��������� ǥ���ȴ�.
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 3, IdleTime);
	BodyRenderer->CreateAnimation("Walk", MonsterStr, 0, 3, RunnigTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", MonsterStr, 6, 10, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", MonsterStr, 11, 13, AttackFrame);
	BodyRenderer->CreateAnimation("AttackRecovery", MonsterStr, 0, 3, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 14, 14, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 15, 17, DeathTime, false);


	BodyRenderer->ChangeAnimation("Idle");
}

void AAspidMother::SetAttack(float _DeltaTime)
{
	SetAttackRendererOffset();
	CheckDeath();
	ActiveGravity();

	//Dash();

	// �����Լ�
	//CreateAttackEffect();

	// �߰��� ����
	if (false == bIsSpawn)
	{
		SoundPlay("hatcher_give_birth.wav");
		SpawnAspidHatchling();
	}

	AttackFrameElapsed += _DeltaTime;
	//   �ǰݽ�                        �˹��� ����Ǵ� ģ������ ��� ��ų ĵ��
	if (true == Stat.IsBeingHit() && true == Stat.IsKnockbackable())
	{
		bIsSpawn = false;
		bIsFirstIdle = true; // Idle�� ���ư��� �ݵ�� �־��ֱ�
		FSM.ChangeState(EMonsterState::IDLE);
	}
	else if (AttackFrameElapsed >= AttackDuration)
	{
		bIsSpawn = false;
		AttackFrameElapsed = 0.0f;
		FSM.ChangeState(EMonsterState::ATTACK_RECOVERY);
	}
}

void AAspidMother::SpawnAspidHatchling()
{
	bIsSpawn = true;

	AAspidHatchling* Child = GetWorld()->SpawnActor<AAspidHatchling>().get();
	FVector MotherPos = GetActorLocation();
	ChildZSort += 1.0f;
	Child->SetActorLocation({ MotherPos.X, MotherPos.Y - 100.0f, ChildZSort });
	Child->SetParentRoom(ParentRoom);
	ParentRoom->GetMonstersRef().push_back(Child);

}

void AAspidMother::DeathPattern()
{
	if (false == bIsSpawn)
	{
		SpawnAspidHatchling();
		SpawnAspidHatchling();
	}
}
