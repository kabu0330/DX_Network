#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"
#include <EngineBase/EngineRandom.h>
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"

AMonster::AMonster()
{
	SetName("Monster");
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	// Renderer
	UEngineRandom Random;
	Random.SetSeed(reinterpret_cast<__int64>(this));
	float Result = Random.Randomfloat(0.0f, 99.9f);
	ZSort = static_cast<float>(EZOrder::MONSTER) + Result;
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	BodyRenderer->BillboardOn();

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

void AMonster::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 20;
	Data.Hp = 15;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 200.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = InitJumpForce;
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = true; // �����ϴ� ���͸� true
	bIsAggressive = true; // ȣ�����̸� true

}

void AMonster::SetOffset()
{
	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	//GravityPointOffset.Y = 1973.0f / 2.0f; // (�̹��� ũ�� - 1������ ũ��) / 2.0f
	WallPointOffest = { -1394.0f / 2.0f, GravityPointOffset.Y - 130.0f };

	DetectRange = { 700, 50 };
}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();
	Knight = AKnight::GetPawn();

	SetOffset();
	SetStatus();
	CreateAnimation();
	CreateCollision();
	CreateCenterPoint();
	CreateDetectCollision();
	CreatePixelCollision();
	SetCollisionEvent(); 
	SetFSM();

	InitBodyCollisionOffset = BodyCollisionOffset;
	BodyRenderer->SetRelativeLocation({ RendererOffset.X, RendererOffset.Y, ZSort });
}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CheckCurRoom(); // ���� ����Ʈ�� ���� �� ��ġ�� ��� üũ
	SetPause(); // ����Ʈ�� ���Ͱ� ���� ��� ��ġ���� ������ bIsPause�� ����
	ActivePixelCollision(); // �ȼ� �浹

	Knockback(_DeltaTime);
	ReverseForce(_DeltaTime);

	TimeElapsed(_DeltaTime); // ��Ÿ�� ���

	UpdateFSM(_DeltaTime);

	DebugInput(_DeltaTime);

}

void AMonster::TimeElapsed(float _DeltaTime)
{
	if (true == IsPause())
	{
		return;
	}

	if (true == bCanMove) // �̵� ��Ÿ��
	{
		if (false == bIsChasing) // �÷��̾� ���� ���°� �ƴ� ���� �̵� ��Ÿ�� ����
		{
			MoveElapsed += _DeltaTime;
			if (MoveElapsed >= MoveDuration)
			{
				bCanMove = false;
				MoveElapsed = 0.0f;

				TimeEventer->AddEndEvent(MoveCooldown, [this]()
					{
						bCanMove = true;
						bChooseDirection = false; // ���� ���� ����
					});
			}
		}
	}

	if (true == bIsChasing) // ���� ���� ���� �¿���� ����
	{
		if (true == bIsChangeChasingDir)
		{
			ChasingDirectionElapsed += _DeltaTime;
			if (ChasingDirectionElapsed >= ChasingDirectonCooldown)
			{
				ChasingDirectionElapsed = 0.0f;
				bIsChangeChasingDir = false;
			}
		}
	}

	if (false == bCanAttack) // ���� ��Ÿ�� ����
	{
		AttackElapsed += _DeltaTime;
		if (AttackElapsed >= AttackCooldown)
		{
			AttackElapsed = 0.0f;
			bCanAttack = true;
		}
	}

}

void AMonster::ResetAttackCooldown()
{
	bCanAttack = true;
	AttackElapsed = 0.0f;
}

bool AMonster::IsPlayerNearby()
{
	if (true == IsPause()) 
	{
		return false;
	}
	if (false == bIsAggressive)
	{
		return false;
	}
	if (nullptr == DetectCollision) // Ž�� ������ ������ ����
	{
		return false;
	}

	FVector KnightPos = Knight->GetActorLocation();
	FVector MonsterPos = this->GetActorLocation();
	FVector Distance = KnightPos - MonsterPos;

	float Length = Distance.Length();
	float DetectLength = DetectCollision->GetWorldScale3D().Half().Length();

	if (DetectLength >= Length)
	{
		return true;
	}
	return false;
}

void AMonster::Dash()
{
	if (true == bIsWallHere)
	{
		return;
	}		
	CheckDirection(); // �¿� ���� ����
	float DeltaTime = UEngineCore::GetDeltaTime();
	AddActorLocation(Direction * Stat.GetRunSpeed() * DeltaTime);
}

void AMonster::Jump(float _DeltaTime)
{
	if (true == bIsCeilHere)
	{
		return;
	}

	CheckDirection(); // �¿� ���� ����
	JumpForce += InitJumpForce;
	if (JumpForce >= JumpForceMax)
	{
		JumpForce = JumpForceMax;
	}
	AddActorLocation({ 0.0f, JumpForce * _DeltaTime });
}

FVector AMonster::GetDirectionToPlayer()
{
	if (true == IsPause())
	{
		return FVector::ZERO;
	}
	if (false == bIsChasing)
	{
		return FVector::ZERO;
	}
	if (true == bIsChangeChasingDir)
	{
		return FVector::ZERO;
	}

	bIsChangeChasingDir = true; // ���� �ֱ⸶�� �������� ���


	CheckDirectionToPlayer();

	return Direction;
}

void AMonster::CheckDirectionToPlayer()
{
	FVector KnightPos = Knight->GetActorLocation();
	FVector MonsterPos = this->GetActorLocation();

	FVector Distance = KnightPos - MonsterPos;
	Distance.Z = 0.0f;
	if (false == bCanFly)
	{
		Distance.Y = 0.0f;

	}
	Distance.Normalize();
	Direction = Distance;


	if (Direction.X <= 0)
	{
		bIsLeft = true;
	}
	else if (Direction.X > 0)
	{
		bIsLeft = false;
	}
}

FVector AMonster::GetRandomDirection()
{
	if (false == bIsFirstIdle)
	{
		return FVector::ZERO;
	}
	if (true == IsPause())
	{
		return FVector::ZERO;
	}
	if (true == bIsChasing)
	{
		return FVector::ZERO;
	}
	if (true == bChooseDirection)
	{
		return FVector::ZERO; // ���� �̵����� ���� ����������.
	}
	if (true == IsPlayerNearby()) // �÷��̾ ��ó�� ������ ���� ���� ����������.
	{
		return FVector::ZERO;
	}
	FVector LeftTop = FVector::LEFT + FVector::UP;
	LeftTop.Normalize();

	FVector LeftBot = FVector::LEFT + FVector::DOWN;
	LeftBot.Normalize();

	FVector RightTop = FVector::RIGHT + FVector::UP;
	RightTop.Normalize();

	FVector RightBot = FVector::RIGHT + FVector::DOWN;
	RightBot.Normalize();

	UEngineRandom Random;
	int Result = -1;
	if (false == bCanFly) // �� �� ���� �ִ� �¿� �̵���
	{
		Result = Random.RandomInt(0, 1);
	}
	else // �� �� ������ 6����
	{
		Result = Random.RandomInt(0, 5);
	}

	switch (Result)
	{
	case 0:
	{
		Direction = FVector::LEFT;
		bIsLeft = true;
		break;
	}
	case 1:
	{
		Direction = FVector::RIGHT;
		bIsLeft = false;
		break;
	}
	case 2:
	{
		Direction = LeftTop;
		bIsLeft = true;
		break;
	}
	case 3:
	{
		Direction = LeftBot;
		bIsLeft = true;
		break;
	}
	case 4:
	{
		Direction = RightTop;
		bIsLeft = false;
		break;
	}
	case 5:
	{
		Direction = RightBot;
		bIsLeft = false;
		break;
	}
	default:
		break;
	}
	bIsFirstIdle = false;
	return Direction;
}

void AMonster::CheckDeath()
{
	if (true == IsPause())
	{
		return;
	}
	if (0 >= Stat.GetHp())
	{
		FSM.ChangeState(EMonsterState::DEATH_AIR);
	}
}

void AMonster::ReverseForce(float _DeltaTime)
{
	if (FVector::ZERO == Stat.GetKnockbackForce())
	{
		return;
	}
	bIsKnockback = true;
	FVector Reverse = -Stat.GetKnockbackForce();
	Reverse.Normalize();

	if (50.0f >= Stat.GetKnockbackForce().Length())
	{
		bIsKnockback = false;
		Stat.SetKnockbackDir(FVector::ZERO);
	}

	if (true == bIsWallHere)
	{
		return;
	}
	if (true == bIsBarrier)
	{
		return;
	}

	Stat.AddKnockbackForce(Reverse * _DeltaTime * 500.0f);
}

void AMonster::Knockback(float _DeltaTime)
{
	if (true == IsPause())
	{
		return;
	}
	if (true == Stat.IsDeath())
	{
		return;
	}
	if (true == bIsWallHere)
	{
		return;
	}
	if (true == bIsBarrier)
	{
		return;
	}
	if (false == Stat.IsKnockbackable())
	{
		return;
	}
	if (FVector::ZERO != Stat.GetKnockbackForce())
	{
		ARoom* Room = ARoom::GetCurRoom();
		bool Result = Room->CheckPixelCollisionWithWall(this, Stat.GetKnockbackForce().X, !bIsLeft);
		if (false == Result)
		{
		
		}
		AddActorLocation(Stat.GetKnockbackForce() * _DeltaTime);
	}
}

void AMonster::DeathAir(float _DeltaTime)
{
	// 1. �з��� ����
	FVector KnightPos = Knight->GetActorLocation();
	FVector MonsterPos = GetActorLocation();
	FVector Direction = MonsterPos - KnightPos;
	Direction.Normalize();

	// 2. ���������� ���ư����� ���� �߰� 
	Direction += FVector::UP;
	Direction.Normalize();

	// 3. �˹� �� �Է�
	Stat.SetKnockbackDir(Direction);
	Stat.SetKnockbackDistance(DeathAirDistance);

	// 4. �߷� �� ƨ�� ���� �߰�
	if (false == bIsOnGround)
	{
		// �߷��� ����� �˹�
		FVector CurrentForce = Stat.GetKnockbackForce();
		CurrentForce += GravityForce;

		// �ִ� �� ����
		if (CurrentForce.Y < -3000.0f) // �߷¿� ���� �ӵ� ����
		{
			CurrentForce.Y = -3000.0f;
		}
		Stat.SetKnockbackForce(CurrentForce);

		AddActorLocation(CurrentForce * _DeltaTime);
	}
}

void AMonster::Death(float _DeltaTime)
{
}

void AMonster::Move(float _DeltaTime)
{
	if (true == IsPause())
	{
		return;
	}
	if (true == bIsWallHere)
	{
		return;
	}
	bChooseDirection = true; // true�� ���� �׸� �ٲ�
	FVector FinalVelocity = FVector::ZERO;
	if (false == bCanFly)
	{
		FinalVelocity = FVector(Stat.GetVelocity() * _DeltaTime, 0.0f);
	}
	else // �ϴ��� ���� ���ʹ� y�൵ �̵��� �� �ִ�.
	{
		FinalVelocity = FVector(Stat.GetVelocity() * _DeltaTime, Stat.GetVelocity() * _DeltaTime * 0.6f);
	}

	FinalVelocity *= Direction;

	AddActorLocation(FinalVelocity);

	if (true == bIsBarrier)
	{
		FVector Distance = BarrierPos - GetActorLocation();
		if (Distance.X < 0) // �踮� ���ʿ� �ִٸ�
		{
			AddRelativeLocation({ 5.0f, 0.0f });
		}
		else
		{
			AddRelativeLocation({ -5.0f, 0.0f });
		}
	}
}

void AMonster::DebugInput(float _DeltaTime)
{
	if (UEngineInput::IsDown(VK_F3))
	{
		SwitchDebugPause();
	}
}

bool AMonster::IsCurRoom()
{
	if (CurRoom == ParentRoom)
	{
		return true;
	}
	return false;
}

bool AMonster::IsPause()
{
	if (false == IsCurRoom()) // �÷��̾ �ٸ� �ʿ� ������ �ൿ�Ұ�
	{
		return true;
	}
	if (true == APlayGameMode::GetPauseRef()) // �������¸� 
	{
		return true;
	}
	if (true == bIsPause)
	{
		return true;
	}
	if (true == bIsDebugPause)
	{
		return true;
	}
	if (nullptr == Knight)
	{
		return true;
	}
	return false;
}

// �Ͻ������ų�, �׾��ų�, ���� ���̰ų�, ���� ���¸� true
bool AMonster::CanAction()
{
	if (true == IsPause()) // ���� ���Ͱ� ���� ���¸�
	{
		return false;
	}
	if (true == Stat.IsDeath()) // �׾�����
	{
		return false;
	}
	if (true == Stat.IsAttacking()) // �������̸�
	{
		return false;
	}
	if (true == Stat.IsStun())
	{
		return false;
	}

	return true;
}

bool AMonster::CanJump()
{
	if (false == CanAction())
	{
		return false;
	}
	if (false == bCanJump)
	{
		return false;
	}

	return true;
}

void AMonster::CheckDirection()
{
	if (false == bCanRotation)
	{
		return;
	}
	if (bIsPrevLeft != bIsLeft)
	{
		bIsPrevLeft = bIsLeft;
		//bIsTurn = true;
	}

	if (false == bIsFlip)
	{
		if (bIsLeft == true)
		{
			SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
			BodyCollisionOffset.X = InitBodyCollisionOffset.X;
			return;
		}
		if (bIsLeft == false)
		{
			SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
			float Offset = BodyCollisionOffset.X * 1.5f;
			BodyCollisionOffset.X = Offset;
			return;
		}
	}

	// �¿����
	if (true == bIsFlip)
	{
		if (bIsLeft == true)
		{
			SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
			return;
		}
		if (bIsLeft == false)
		{
			SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
			return;
		}
	}
}

void AMonster::SetPause()
{
	if (true == IsCurRoom())
	{
		bIsPause = false;
	}
	else
	{
		bIsPause = true;
	}
}

void AMonster::CheckCurRoom()
{
	ARoom* MainPawnRoom = ARoom::GetCurRoom();
	CurRoom = MainPawnRoom;
}

