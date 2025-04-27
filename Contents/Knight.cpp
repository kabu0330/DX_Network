#include "PreCompile.h"
#include "Knight.h"

#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/Collision.h>

#include "KnightSkill.h"
#include "Room.h"
#include "PlayGameMode.h"
#include "Door.h"
#include <math.h>
#include "PlayHUD.h"


AKnight* AKnight::MainPawn = nullptr;

AKnight::AKnight()
{
	SetName("AKnight");
	MainPawn = this;

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();

	CreateRenderer();
	CreateCollision();
	SetStatus();

#ifdef _DEBUG
	//SetActorLocation({9911, -5450});
//SetActorLocation({13900, -5360}); // 왼쪽
//SetActorLocation({14800, -5360}); // 오른쪽
//SetActorLocation({1100, -2950});
	SetActorLocation({ 11600, -8000 });
#else
	SetActorLocation(InitPos::KnightInitPos);
#endif // _DEBUG


	// Debug
	BodyRenderer->BillboardOn();

}

void AKnight::BeginPlay()
{
	AActor::BeginPlay();
	SetFSM();
	SetCollisionEvent();
	CreatePixelCollision();
	//SetCameraPos();
}

void AKnight::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	ActivePixelCollsion();

	SetCameraPos();
	//CheckCameraPos();
	//SetCameraLerp();
	CheckEnterButton();

	UpdateFSM(_DeltaTime);
	TimeElapsed(_DeltaTime);

	ReverseForce(_DeltaTime);
	Knockback(_DeltaTime);

	DebugInput(_DeltaTime);
	DebugCamera();

}

void AKnight::SetCollisionEvent()
{
	BodyCollision->SetCollisionEnter(std::bind(&AKnight::Collide, this, std::placeholders::_1, std::placeholders::_2));

	BodyCollision->SetCollisionStay(std::bind(&AKnight::CheckEnterDoor, this, std::placeholders::_1, std::placeholders::_2));
}

void AKnight::Collide(UCollision* _This, UCollision* _Other)
{
}

void AKnight::DeathCheck()
{
	if (0 >= Stat.GetHp())
	{
		if (true == Stat.IsDeath())
		{
			return;
		}
		Stat.SetDeath(true);
	}
}

void AKnight::ResetLevel()
{
	APlayHUD* HUD = dynamic_cast<APlayHUD*>(GetWorld()->GetHUD());
	HUD->FadeIn();
	TimeEventer->AddEndEvent(0.5f, [this]()
		{
			UEngineCore::ResetLevel<APlayGameMode, AKnight, APlayHUD>("Play");
			UEngineSound::AllSoundOff();
			
		});

}

void AKnight::UpdateFSM(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
}

void AKnight::ActiveGravity()
 {
	if (true == NoneGravity)
	{
		return;
	}

	ARoom* Room = ARoom::GetCurRoom();
	if (nullptr != Room)
	{
		Room->CheckPixelCollisionWithGravity(this, FVector::DOWN);
	}
}

void AKnight::ActivePixelCollsion()
{
	if (true == NoneGravity)
	{
		return;
	}
	ARoom* Room = ARoom::GetCurRoom();
	if (nullptr != Room)
	{
		Room->CheckPixelCollisionWithWall(this, Stat.GetVelocity(), bIsLeft, FVector::ZERO);
		Room->CheckPixelCollisionWithCeil(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft, FVector::ZERO);
	}
}

void AKnight::CreatePixelCollision()
{
	PixelCollision = CreateDefaultSubObject<UCollision>();
	//std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	PixelCollision->SetupAttachment(RootComponent);
	PixelCollision->SetScale3D({ 70, 5 });
	PixelCollision->SetRelativeLocation({0.0f, -BodyRenderer->GetScale().Y * 0.5f, });
	PixelCollision->SetCollisionProfileName("MonsterPoint");
	PixelCollision->SetDebugColor({ 1.0f, 1.0f, 1.0f });
}

void AKnight::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 400.0f;
	Data.InitVelocity = Data.Velocity;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 5;
	Data.Hp = 5;
	Data.MaxMp = 99;
	Data.Mp = 99;
	Data.Att = 5;
	Data.SpellAtt = 15;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 300.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	InitKnockbackDistance = Data.KnockbackDistance;
	JumpForce = InitJumpForce;
	bCanRotation = true;
}

void AKnight::Move(float _DeltaTime)
{
	if (false == bIsDashing)
	{
		Stat.SetVelocity(Stat.GetInitVelocity());

	}
	if (UEngineInput::IsDown('C'))
	{
		Stat.SetVelocity(Stat.GetDashSpeed());
		//Velocity = DashSpeed;
	}

	if (false == CanMove())
	{
		return;
	}

	CheckDirection();

	FVector MoveVector = FVector::ZERO;
	
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		if (false == bIsWallHere)
		{
			MoveVector.X -= Stat.GetVelocity() * _DeltaTime;
		}
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		if (false == bIsWallHere)
		{
			MoveVector.X += Stat.GetVelocity() * _DeltaTime;
		}
	}	

	AddRelativeLocation(MoveVector);

	if (true == bIsBarrier)
	{
		FVector Distance = BarrierPos - GetActorLocation();
		if (Distance.X < 0) // 배리어가 왼쪽에 있다면
		{
			AddRelativeLocation({5.0f, 0.0f});
		}
		else
		{
			AddRelativeLocation({-5.0f, 0.0f});
		}
	}
}

void AKnight::ReverseForce(float _DeltaTime)
{
	if (false == Stat.IsKnockbackable())
	{
		return;
	}
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
		Stat.SetKnockbackDir(FVector::ZERO);
		return;
	}
	if (true == bIsBarrier)
	{
		Stat.SetKnockbackDir(FVector::ZERO);
		return;
	}

	Stat.AddKnockbackForce(Reverse * _DeltaTime * 500.0f);

}

void AKnight::Knockback(float _DeltaTime)
{
	if (false == Stat.IsKnockbackable())
	{
		return;
	}
	if (FVector::ZERO != Stat.GetKnockbackForce())
	{
		//ARoom* Room = ARoom::GetCurRoom();
		//bool Result = Room->CheckPixelCollisionWithWall(this, Stat.GetKnockbackForce().X, !bIsLeft);
		//if (false == Result)
		//{
		//	
		//}
		AddActorLocation(Stat.GetKnockbackForce() * _DeltaTime);
	}
}

void AKnight::SetCameraPos()
{
	bIsCameraMove = false;
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	FVector KnightPos = GetActorLocation();
	FVector ScreenSize = UEngineCore::GetScreenScale();
	ScreenRatioY = 0.15f;
	Camera->SetActorLocation({ KnightPos.X, KnightPos.Y + ScreenSize.Y * ScreenRatioY });
	FVector Pos = Camera->GetActorLocation();
}

void AKnight::CheckCameraPos()
{
	if (bIsCameraMove == true)
	{
		return;
	}

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	FVector KnightPos = GetActorLocation();
	FVector CameraPos = Camera->GetActorLocation();
	FVector ScreenSize = UEngineCore::GetScreenScale();

	float Distance = ::abs(KnightPos.Length() - CameraPos.Length());
	float Width = ::abs(KnightPos.X - CameraPos.X);
	float Height = ::abs(KnightPos.Y - (CameraPos.Y - ScreenSize.Y * ScreenRatioY));
	float KnightPosY = KnightPos.Y - BodyRenderer->GetScale().Y * 0.5f;
	float Rs = CameraPos.Y - ScreenSize.Y * ScreenRatioY;

	if (Height > 200.0f || Width > 100.0f)
	{
		CameraCurPos = CameraPos;
		
		float TargetY = KnightPos.Y + ScreenSize.Y * ScreenRatioY;
		float ClampPosY = UEngineMath::Clamp(TargetY, (CameraPos.Y - 120.0f) , (CameraPos.Y + 0.0f));
		CameraTargetPos = { KnightPos.X, ClampPosY };

		bIsCameraMove = true;
	}
}

void AKnight::SetCameraLerp()
{
	if (false == bIsCameraMove)
	{
		return;
	}

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	float CameraMoveDuration = 0.2f;
	float Alpha = CameraMoveTime / CameraMoveDuration;

	FVector CameraDest = UEngineMath::Lerp(CameraCurPos, CameraTargetPos, Alpha);
	Camera->SetActorLocation(CameraDest);


	float DeltaTime = UEngineCore::GetDeltaTime();
	CameraMoveTime += DeltaTime;
	if (CameraMoveDuration <= CameraMoveTime)
	{
		CameraMoveTime = 0.0f;
		bIsCameraMove = false;
	}

	FVector CameraResult = GetActorLocation();
}

void AKnight::TimeElapsed(float _DeltaTime)
{
	if (true == ARoom::GetActiveGravity())
	{
		return;
	}
	float AttackCooldown = 0.5f;

	if (true == Stat.IsAttacking())
	{
		AttackCooldownElapsed += _DeltaTime;
		if (AttackCooldownElapsed >= AttackCooldown)
		{
			Stat.SetAttacking(false);
			bIsShowEffect = false;
			AttackCooldownElapsed = 0.0f;
		}
	}

	float DashCooldown = 0.3f;
	if (false == bCanDash)
	{
		DashCooldownElapsed += _DeltaTime;
		if (DashCooldownElapsed >= DashCooldown)
		{
			if (true == bIsOnGround)
			{
				bCanDash = true;
				DashCooldownElapsed = 0.0f;
			}
		}
	}

	// 무적 해제
	if (true == bIsInvincible)
	{
		InvincibleElapsed += _DeltaTime;
		if (InvincibleElapsed >= InvincibleDuration)
		{
			InvincibleElapsed = 0.0f;
			bIsInvincible = false;
		}
	}
}

void AKnight::CheckEnterButton()
{
	if (UEngineInput::IsUp(VK_UP))
	{
		bIsEnter = false;
	}
}

void AKnight::CheckEnterDoor(UCollision* _This, UCollision* _Target)
{
	AActor* TargetActor =  _Target->GetActor();
	ADoor* Door = dynamic_cast<ADoor*>(TargetActor);
	if (nullptr == Door)
	{
		return;
	}

	if (UEngineInput::IsDown(VK_UP))
	{
		bIsEnter = true;
	}
}

bool AKnight::CanAction()
{
	if (true == Stat.IsDeath())
	{
		return false;
	}
	if (true == Stat.IsCastingSpell())
	{
		return false;
	}
	if (true == Stat.IsAttacking())
	{
		return false;
	}

	return true;
}

bool AKnight::CanMove()
{
	if (true == Stat.IsDeath())
	{
		return false;
	}
	if (true == Stat.IsCastingSpell())
	{
		return false;
	}
	if (true == Stat.IsBeingHit())
	{
		return false;
	}
	if (true == bIsDashing)
	{
		return false;
	}
	if (false == bCanMove)
	{
		return false;
	}

	return true;
}

void AKnight::RecoveryIdle()
{
	bCanRotation = true;
	bIsFireballEffect = false;
	bIsStunEffect = false;

	Stat.SetStun(false);
	Stat.SetKnockbackDistance(InitKnockbackDistance);

	bIsSound = false;

	//bIsStun = false;
}

void AKnight::ChangeDash()
{
	if (false == CanAction())
	{
		return;
	}

	if (false == bCanDash)
	{
		return;
	}
	if (UEngineInput::IsDown('C'))
	{
		bIsDashing = true;
		FSM.ChangeState(EKnightState::DASH);
		return;
	}
}

void AKnight::Dash()
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	bCanRotation = false;
	bIsDashing = true;
	bCanDash = false;

	if (false == bIsWallHere)
	{
		if (true == bIsBarrier)
		{
			return;
		}

		if (true == bIsLeft)
		{
			AddRelativeLocation(FVector{ -Stat.GetVelocity() * DeltaTime, 0.0f, 0.0f });
		}
		else
		{
			AddRelativeLocation(FVector{ Stat.GetVelocity() * DeltaTime, 0.0f, 0.0f });
		}
	}
}

void AKnight::CastFocus()
{
	if (false == CanAction())
	{
		return;
	}
	if (22 > Stat.GetMp())
	{
		return;
	}
	if (false == bIsOnGround)
	{
		return;
	}
	if (UEngineInput::IsPress('A'))
	{
		float PressTime = UEngineInput::IsPressTime('A');
		float TriggerTime = 0.3f;

		if (PressTime >= TriggerTime)
		{
			FSM.ChangeState(EKnightState::FOCUS);
			return;
		}
	}
}


void AKnight::CastFireball()
{
	if (false == CanAction())
	{
		return;
	}
	if (22 > Stat.GetMp())
	{
		return;
	}
	if (UEngineInput::IsUp('A'))
	{
		Stat.AddMp(-33);
		FSM.ChangeState(EKnightState::FIREBALL_ANTIC);
		return;
	}
}

bool AKnight::CanJump()
{
	if (false == CanAction())
	{
		return false;
	}
	if (false == bIsOnGround)
	{
		return false;
	}

	return true;
}

void AKnight::Jump(float _DeltaTime)
{
	if (true == CanAction())
	{
		if (true == bIsCeilHere)
		{
			return;
		}
		if (UEngineInput::IsPress('Z'))
		{		
			float JumpAccTime = 0.3f;
			float JumpKeyDuration = UEngineInput::IsPressTime('Z');
			if (JumpAccTime >= JumpKeyDuration)
			{
				JumpForce = 1200.0f;
			}
	
			AddRelativeLocation(FVector{ 0.0f, JumpForce * _DeltaTime, 0.0f });	
		}
	}

}

void AKnight::ChangeJumpAnimation()
{
	if (true == CanJump())
	{
		if (UEngineInput::IsPress('Z'))
		{
			Sound.Stop();
			Sound = UEngineSound::Play("jump.wav");
			Sound.SetVolume(0.5f);
			FSM.ChangeState(EKnightState::JUMP);
			return;
		}
	}
}

void AKnight::ChangeNonCombatAnimation()
{
	if (true == CanAction())
	{
		return;
	}
}

void AKnight::ChangeLookAnimation()
{
	//if (UEngineInput::IsPress(VK_DOWN))
	//{
	//	float PressTime = UEngineInput::IsPressTime(VK_DOWN);
	//	float TriggerTime = 0.5f;

	//	if (PressTime >= TriggerTime)
	//	{
	//		FSM.ChangeState(EKnightState::LOOK_DOWN);
	//		return;
	//	}
	//}
	//if (UEngineInput::IsPress(VK_UP))
	//{
	//	float PressTime = UEngineInput::IsPressTime(VK_UP);
	//	float TriggerTime = 0.5f;

	//	if (PressTime >= TriggerTime)
	//	{
	//		FSM.ChangeState(EKnightState::LOOK_UP);
	//		return;
	//	}
	//}
}

void AKnight::DebugInput(float _DeltaTime)
{
	if (true == ARoom::GetActiveGravity())
	{
		bIsOnGround = true;
	}

	if (UEngineInput::IsDown('T'))
	{
		if (true == ARoom::GetActiveGravity())
		{
			//Velocity = 1500.0f;
			//InitVelocity = Velocity;
			//DashSpeed = Velocity * 3.0f;
			Stat.SetVelocity(1500.0f);
			Stat.SetInitVelocity(Stat.GetVelocity());
			Stat.SetDashSpeed(Stat.GetVelocity() * 3.0f);
			bCanDash = true;
			SwitchDebugMode();
		}		
	}

	if (UEngineInput::IsDown('V'))
	{
		FSM.ChangeState(EKnightState::STUN);
		Stat.AddHp(-1);
	}

	if (UEngineInput::IsDown('B'))
	{
		Stat.AddHp(1);
	}

	float ZValue = BodyRenderer->GetTransformRef().RelativeLocation.Z;
	int a = 0;

	if (UEngineInput::IsDown('Q'))
	{
		SwitchActiveGravity();
	}

	// Debug Input
	if (true == ARoom::GetActiveGravity())
	{
		if (UEngineInput::IsPress(VK_LEFT))
		{
			AddRelativeLocation(FVector{ -Stat.GetVelocity() * _DeltaTime,0.0f, 0.0f });
		}
		if (UEngineInput::IsPress(VK_RIGHT))
		{
			AddRelativeLocation(FVector{ Stat.GetVelocity() * _DeltaTime,0.0f, 0.0f });
		}
		if (UEngineInput::IsPress(VK_UP))
		{
			AddRelativeLocation(FVector{ 0.0f, Stat.GetVelocity() * _DeltaTime, 0.0f });
		}
		if (UEngineInput::IsPress(VK_DOWN))
		{
			AddRelativeLocation(FVector{ 0.0f, -Stat.GetVelocity() * _DeltaTime, 0.0f });
		}
	}
}

void AKnight::DebugCamera()
{
	if (false == bIsDebugMode)
	{
		return;
	}
	ACameraActor* Camera = GetWorld()->GetMainCamera().get();
	Camera->SetActorLocation(GetActorLocation());
}

void AKnight::ChangeAttackAnimation(EKnightState _PrevState)
{
	if (true == CanAction())
	{
		if (UEngineInput::IsDown('X') && UEngineInput::IsPress(VK_UP))
		{
			Stat.SetAttacking(true);
			//bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::UP_SLASH);
			return;
		}
		if (UEngineInput::IsDown('X') && UEngineInput::IsPress(VK_DOWN) && false == bIsOnGround)
		{
			Stat.SetAttacking(true);
			//bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::DOWN_SLASH);
			return;
		}
		if (UEngineInput::IsDown('X'))
		{
			Stat.SetAttacking(true);
			//bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::SLASH);
			return;
		}
	}
}

void AKnight::CreateCollision()
{
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetCollisionProfileName("Knight");
	BodyCollision->SetScale3D({ 60.0f, 130.0f });
	BodyCollision->SetRelativeLocation(BodyRenderer->GetActorLocation());
	//BodyCollision->SetCollisionType(ECollisionType::AABB);

}

void AKnight::CheckDirection()
{
	if (false == CanAction()) 
	{
		return;
	}
	if (false == bCanRotation)
	{
		return;
	}
	if (UEngineInput::IsPress(VK_LEFT))
	{
		bIsLeft = true;
		SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		bIsLeft = false;
		SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
	}
}

AKnight::~AKnight()
{
	BodyRenderer = nullptr;
	BodyCollision = nullptr;
}
