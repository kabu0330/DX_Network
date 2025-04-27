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
	Data.DashSpeed = Data.Velocity * 0.0f; // 공격 중에 안움직이려고
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
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true
	bCanFly = true; // 날아다니는 몬스터라면 true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackFrame = 0.15f;// 매번 설정
	AttackDuration = AttackFrame * 3.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.01f;

	bIsDeathDestroy = true; // 죽으면 시체 없이 소멸할건지

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
	float DeathAirTime = 0.5f; // 날아가는 시간 조절에 따라서 자연스러움이 표현된다.
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

	// 가상함수
	//CreateAttackEffect();

	// 추가된 로직
	if (false == bIsSpawn)
	{
		SoundPlay("hatcher_give_birth.wav");
		SpawnAspidHatchling();
	}

	AttackFrameElapsed += _DeltaTime;
	//   피격시                        넉백이 적용되는 친구들은 모두 스킬 캔슬
	if (true == Stat.IsBeingHit() && true == Stat.IsKnockbackable())
	{
		bIsSpawn = false;
		bIsFirstIdle = true; // Idle로 돌아갈때 반드시 넣어주기
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
