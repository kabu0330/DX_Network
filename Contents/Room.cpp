#include "PreCompile.h"
#include "Room.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"
#include "Monster.h"
#include "Door.h"
#include "Barrier.h"

ARoom::ARoom()
{
	// 항상 DefaultSceneComponent는 생성자에서만. BeginPlay에서는 만들 수 없다.
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BackgroundRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BackgroundRenderer->SetName("Background");
	BackgroundRenderer->SetupAttachment(RootComponent);
}

ARoom::~ARoom()
{
}

void ARoom::BeginPlay()
{
	AActor::BeginPlay();

	if ("FalseKnightRoom" == GetName())
	{
		Collision = CreateDefaultSubObject<UCollision>().get();
		Collision->SetupAttachment(RootComponent);
		Collision->SetCollisionProfileName("Door");
		float ZSort = static_cast<float>(EZOrder::BACKGROUND);
		Collision->GetTransformRef().Location.Z = ZSort;
		FVector CollisionScale = FVector(500, 900);
		Collision->SetScale3D(CollisionScale);
		Collision->SetWorldLocation({ 12200, -7900 });
		if (nullptr != Collision)
		{
			Collision->SetCollisionEnter(std::bind(&ARoom::BossSpawn, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
 }

void ARoom::BossSpawn(UCollision* _This, UCollision* _Other)
{
	Collision->SetActive(false);
	for (AMonster* Monster : Monsters)
	{
		Monster->SetActive(true);
	}
}

void ARoom::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	SoundPlay();
}

void ARoom::CreateTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);

	BackgroundRenderer->SetTexture(_FileName, true, _ScaleRatio);
}

void ARoom::SetRoomLocation(FVector _Pos)
{
	float ZOrder = static_cast<float>(EZOrder::BACKGROUND);
	SetActorLocation({ GetActorLocation().X + _Pos.X, GetActorLocation().Y + _Pos.Y, ZOrder });
	LeftTopPos = _Pos;
}

// 좌상단을 0, 0 기준으로 변경
ADoor* ARoom::CreateDoor(FVector _DoorScale, FVector _InitPos, ARoom* _TargetRoom, FVector _TargetPos, bool _IsDoor)
{
	Door = GetWorld()->SpawnActor<ADoor>().get();

	FVector RoomPos = { GetActorLocation().X - (Size.X / 2.0f), GetActorLocation().Y + (Size.Y / 2.0f) };
	Door->SetActorLocation( _InitPos);

	Door->SetScale(_DoorScale);
	Door->SetWarpPosition(_TargetRoom, _TargetPos, _IsDoor);
	Door->GetCollision()->SetDebugColor({ 1.0f, 0.0f, 1.0f, 1.0f });
	return Door;
}

// 중력
void ARoom::CheckPixelCollisionWithGravity(AActor* _Actor, FVector _Offset)
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	FVector TwoPixelUp = FVector::UP * 2.0f;

	FVector CollisionPoint = GetPixelCollisionPoint(_Actor, _Offset);
	FVector CollisionPoint2PixelUp = GetPixelCollisionPoint(_Actor, _Offset + TwoPixelUp);
	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		if (true == IsOnGround(CollisionPoint))
		{
			Knight->SetOnGround(true);

			while (true == IsOnGround(CollisionPoint2PixelUp))
			{
				Knight->AddRelativeLocation(TwoPixelUp);
				CollisionPoint2PixelUp = GetPixelCollisionPoint(_Actor, _Offset + TwoPixelUp);
			}
		}
		else
		{
			Knight->SetOnGround(false);
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (true == IsOnGround(CollisionPoint))
		{
			Monster->SetOnGround(true);
			if (true == Monster->CanFly())
			{
				while (true == IsOnGround(CollisionPoint2PixelUp))
				{
					Monster->AddRelativeLocation(TwoPixelUp);
					{
						CollisionPoint2PixelUp = GetPixelCollisionPoint(_Actor, _Offset + TwoPixelUp);
					}
				}
			}
		}
		else
		{
			Monster->SetOnGround(false);
		}
	}

	Force(_Actor, DeltaTime);
}

FVector ARoom::GetPixelCollisionPoint(AActor* _Actor, FVector _Offset)
{
	float DeltaTime = UEngineCore::GetDeltaTime();

	FVector GravityForce = FVector::ZERO;
	FVector CollisionPos = FVector::ZERO;
	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		CollisionPos = Knight->GetPixelCollision()->GetWorldLocation();
		GravityForce = Knight->GetGravityForce();
	}
	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		CollisionPos = Monster->GetPixelCollision()->GetWorldLocation();
		GravityForce = Monster->GetGravityForce();
	}
	ASkill* Skill = dynamic_cast<ASkill*>(_Actor);
	if (nullptr != Skill)
	{
		CollisionPos = Skill->GetCollision()->GetWorldLocation();
	}

	CollisionPos -= LeftTopPos;
	FVector NextPos = GravityForce * DeltaTime;

	FVector CollisionPoint = { CollisionPos.X + NextPos.X + _Offset.X, CollisionPos.Y + NextPos.Y + _Offset.Y };

	CollisionPoint.RoundVector();

	return CollisionPoint;
}


bool ARoom::IsOnGround(FVector _Pos)
{
	FVector CollisionPoint = _Pos;

	CollisionPoint.RoundVector();

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	if (CollisionColor == UColor::BLACK || CollisionColor == UColor(0, 0, 0, 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

ABarrier* ARoom::CreateBarrier(FVector _Pos)
{
	ABarrier* Barrier = GetWorld()->SpawnActor<ABarrier>().get();
	Barrier->SetActorLocation(_Pos);
	Barriers.push_back(Barrier);
	return Barrier;
}

void ARoom::Force(AActor* _Actor, float _DeltaTime)
{
	if (true == bActiveGravity)
	{
		return;
	}
	float GravityValue = 1200.0f;
	float GravityMaxValue = 1500.0f;
	FVector PrevForce = FVector::ZERO;
	FVector GravityForceMax = FVector::DOWN * GravityMaxValue;

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		FVector GravityForce = Knight->GetGravityForce();
		if (false == Knight->IsOnGround())
		{
			PrevForce = GravityForce;
			GravityForce += FVector::DOWN * GravityValue * _DeltaTime;
		}
		else
		{
			GravityForce = FVector::ZERO;
		}

		if (abs(GravityForceMax.Y) <= abs(GravityForce.Y))
		{
			GravityForce = PrevForce;
			Knight->SetHardLand(true);
		}

		Knight->SetGravityForce(GravityForce);
		Knight->AddRelativeLocation(GravityForce * _DeltaTime);
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		FVector GravityForce = Monster->GetGravityForce();

		if (true == Monster->CanFly()) // 날아다니는 몬스터는 중력 미적용
		{
			GravityForce = FVector::ZERO;
			return;
		}

		if (false == Monster->IsOnGround())
		{
			GravityForce += FVector::DOWN * GravityValue * _DeltaTime;
		}
		else
		{
			GravityForce = FVector::ZERO;
		}

		if (GravityValue <= GravityForce.Y)
		{
			GravityForce = FVector::DOWN * GravityMaxValue * _DeltaTime;
		}

		Monster->SetGravityForce(GravityForce);
		Monster->AddRelativeLocation(GravityForce * _DeltaTime);
	}
}

void ARoom::SoundPlay()
{
	if ("" == SoundName)
	{
		return;
	}
	if (this != CurRoom)
	{
		return;
	}
	if (CurSoundName != SoundName)
	{
		UEngineSound::AllSoundStop();

		if ("Dirtmouth" == GetName())
		{
			SoundWind = UEngineSound::Play("dirtmouth_wind_loop_a.wav");
			Sound.Loop(999);
		}
		Sound = UEngineSound::Play(SoundName);
		CurSoundName = SoundName;
		Sound.Loop(999);
		return;
	}
}

void ARoom::CheckPixelCollisionWithWall(AActor* _Actor, float _Speed, bool _Left, FVector _Offset)
{
	if (true == bActiveGravity)
	{
		return;
	}

	FVector CollisionPos = FVector::ZERO;
	FVector CollisionHalfScale = FVector::ZERO;
	bool bIsKnockback = false;

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		CollisionPos = Knight->GetPixelCollision()->GetWorldLocation();
		CollisionHalfScale = Knight->GetPixelCollision()->GetWorldScale3D().Half();
		bIsKnockback = Knight->IsKnockback();
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		CollisionPos = Monster->GetPixelCollision()->GetWorldLocation();
		CollisionHalfScale = Monster->GetPixelCollision()->GetWorldScale3D().Half();
		bIsKnockback = Monster->IsKnockback();
	}

	float DeltaTime = UEngineCore::GetDeltaTime();

	float NextPos = _Speed * DeltaTime;
	CollisionPos -= LeftTopPos;

	FVector CollisionPoint = { CollisionPos.X + NextPos , CollisionPos.Y + 10.0f};

	// 왼쪽, 오른쪽 방향 구분
	if (true == bIsKnockback)
	{
		if (true == _Left)
		{
			CollisionPoint.X += CollisionHalfScale.X;
		}
		else
		{
			CollisionPoint.X -= CollisionHalfScale.X;
		}
	}
	else
	{
		if (true == _Left)
		{
			CollisionPoint.X -= CollisionHalfScale.X;
		}
		else
		{
			CollisionPoint.X += CollisionHalfScale.X;
		}
	}


	// 실수오차 문제 때문에
	CollisionPoint.X = ::roundf(CollisionPoint.X);
	CollisionPoint.Y = ::roundf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::YELLOW || CollisionColor == UColor::BLACK || CollisionColor == UColor::RED)
		{
			Knight->SetWallHere(true);
		}
		else
		{
			Knight->SetWallHere(false);
		}
	}

	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::YELLOW || CollisionColor == UColor::BLACK || CollisionColor == UColor::RED)
		{
			Monster->SetWallHere(true);
		}
		else if (CollisionColor == UColor::WHITE)
		{
			Monster->SetWallHere(false);
		}
		else
		{
			Monster->SetWallHere(true);
		}
	}
}

bool ARoom::CheckPixelCollisionWithWall(AActor* _Actor, float _Speed, bool _Left)
{
	if (true == bActiveGravity)
	{
		return false;
	}

	FVector CollisionPos = FVector::ZERO;
	FVector CollisionHalfScale = FVector::ZERO;
	bool bIsKnockback = false;

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		CollisionPos = Knight->GetPixelCollision()->GetWorldLocation();
		CollisionHalfScale = Knight->GetPixelCollision()->GetWorldScale3D().Half();
		bIsKnockback = Knight->IsKnockback();
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		CollisionPos = Monster->GetPixelCollision()->GetWorldLocation();
		CollisionHalfScale = Monster->GetPixelCollision()->GetWorldScale3D().Half();
		bIsKnockback = Monster->IsKnockback();
	}

	float DeltaTime = UEngineCore::GetDeltaTime();

	float NextPos = _Speed * DeltaTime;
	CollisionPos -= LeftTopPos;

	FVector CollisionPoint = { CollisionPos.X + NextPos , CollisionPos.Y + 20.0f };

	// 왼쪽, 오른쪽 방향 구분
	if (true == bIsKnockback)
	{
		if (true == _Left)
		{
			CollisionPoint.X += CollisionHalfScale.X;
		}
		else
		{
			CollisionPoint.X -= CollisionHalfScale.X;
		}
	}
	else
	{
		if (true == _Left)
		{
			CollisionPoint.X -= CollisionHalfScale.X;
		}
		else
		{
			CollisionPoint.X += CollisionHalfScale.X;
		}
	}


	// 실수오차 문제 때문에
	CollisionPoint.X = ::roundf(CollisionPoint.X);
	CollisionPoint.Y = ::roundf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::YELLOW || CollisionColor == UColor::BLACK || CollisionColor == UColor::RED)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::YELLOW || CollisionColor == UColor::BLACK || CollisionColor == UColor::RED)
		{
			return true;
		}
		else if (CollisionColor == UColor::WHITE)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}


void ARoom::CheckPixelCollisionWithCeil(AActor* _Actor, UContentsRenderer* _Renderer, float _Speed, bool _Left, FVector _Offset)
{
	if (true == bActiveGravity)
	{
		return;
	}

	float DeltaTime = UEngineCore::GetDeltaTime();

	float NextPos = _Speed * DeltaTime;
	FVector ActorPos = _Actor->GetActorLocation() - LeftTopPos;
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.5f + _Offset.Y;

	FVector CollisionPoint = { ActorPos.X + NextPos , ActorPos.Y + HalfRendererHeight };

	// 실수오차 문제 때문에
	CollisionPoint.X = ::roundf(CollisionPoint.X);
	CollisionPoint.Y = ::roundf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::RED)
		{
			Knight->SetCeilHere(true);
		}
		else
		{
			Knight->SetCeilHere(false);
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::RED)
		{
			Monster->SetCeilHere(true);
		}
		else
		{
			Monster->SetCeilHere(false);
		}
	}
}



