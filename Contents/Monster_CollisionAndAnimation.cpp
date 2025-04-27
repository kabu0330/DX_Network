#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"
#include "Particle.h"
#include <EngineBase/EngineRandom.h>

void AMonster::CreateAnimation()
{
	// 재정의하여 사용
}

void AMonster::DamageEffect()
{
	CreateOrangeParticleEffect();
	CreateHitOrangeEffect();
}

void AMonster::CreateOrangeParticleEffect()
{
	AParticle* PuffParticle = GetWorld()->SpawnActor<AParticle>().get();
	FVector ActorPos = GetActorLocation();
	PuffParticle->CreateParticle("Puff", 10, 0.01f, ActorPos);
	PuffParticle->SetParticleOption(EParticleType::RANDOM, -300.0f, 300.0f);

	AParticle* OrangeParticle = GetWorld()->SpawnActor<AParticle>().get();
	OrangeParticle->CreateParticle("OrangeParticle", 20, 0.01f, ActorPos);
	OrangeParticle->SetRandomScale(0.5f, 1.0f);
	OrangeParticle->SetDecayScale(true, 0.6f);
	OrangeParticle->SetParticleOption(EParticleType::RANDOM, -400.0f, 400.0f);

	AParticle* Particle = GetWorld()->SpawnActor<AParticle>().get();
	Particle->CreateParticle("DefaultHitParticle", 20, 0.01f, ActorPos);
	Particle->SetParticleOption(EParticleType::RANDOM, -400.0f, 400.0f);
}

void AMonster::CreateHitOrangeEffect()
{
	FVector ActorPos = GetActorLocation();
	FVector MonsterScale = BodyRenderer->GetScale();

	AEffect* HitOrange = GetWorld()->SpawnActor<AEffect>().get();
	HitOrange->ChangeAnimation("HitOrange", ActorPos);
	UEngineRandom Random;
	int Result = Random.RandomInt(0, 49);
	HitOrange->SetZSort(static_cast<int>(EZOrder::HIT_LIGHT_EFFECT) + Result);
	HitOrange->GetRenderer()->SetAlpha(0.3f);
	HitOrange->SetAlphaFadeInFadeOut(1.5f, 0.7f);
	HitOrange->SetScale(5.0f);
	HitOrange->SetLocation(this);
}

void AMonster::SetCollisionEvent()
{
	BodyCollision->SetCollisionStay(std::bind(&AMonster::OnBodyCollision, this, std::placeholders::_1, std::placeholders::_2));
	DetectCollision->SetCollisionStay([this](UCollision* _This, UCollision* _Other) {
		bIsChasing = true;
		});
	DetectCollision->SetCollisionEnd([this](UCollision* _This, UCollision* _Other) {
		bIsChasing = false;
		});
}

void AMonster::OnBodyCollision(UCollision* _This, UCollision* _Other)
{
	// Debug
	//return;

	AActor* Other = _Other->GetActor();
	AActor* Actor = dynamic_cast<AActor*>(Other);
	if (nullptr == Actor)
	{
		return;
	}
	AKnight* OtherKnight = dynamic_cast<AKnight*>(Actor);
	if (nullptr == OtherKnight)
	{
		return;
	}
	if (true == OtherKnight->IsInvincible())
	{
		return;
	}

	TimeEventer->AddEndEvent(0.3f, [this]()
		{
			Knockback();
		});

	int Att = Stat.GetAtt();
	UFightUnit::OnHit(OtherKnight, Att);
	OtherKnight->SetInvicible(true);

	int KnightCurHp = OtherKnight->GetStatRef().GetHp();
	UEngineDebug::OutPutString(GetName() + "의 공격으로 나이트가 " + std::to_string(Att) + "의 피해를 입었습니다. 남은 체력 : " + std::to_string(KnightCurHp));
}

void AMonster::Knockback()
{
	FVector Pos = { GetActorLocation().X, GetActorLocation().Y };
	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector KnockbackDirection = KnightPos - Pos;
	KnockbackDirection.Y = 0.0f;
	KnockbackDirection.Normalize();
	KnockbackDirection += FVector::UP;
	KnockbackDirection.Normalize();

	Knight->GetStatRef().SetKnockbackDistance(500.0f);
	Knight->GetStatRef().SetKnockbackDir(KnockbackDirection);
}

void AMonster::CreateCollision()
{
	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D(CollisionScale);
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

void AMonster::CreateAttackLogicAndEffect()
{
}

void AMonster::ResetRendererOffset()
{
	BodyRenderer->SetRelativeLocation({ RendererOffset.X, RendererOffset.Y, ZSort });
}

void AMonster::ResetRendererOffset(UContentsRenderer* _Renderer)
{
	_Renderer->SetRelativeLocation({ RendererOffset.X, RendererOffset.Y, ZSort - 1.0f });
}

void AMonster::SetRendererOffset(FVector _Offset)
{
	FVector Offset = _Offset;
	if (true == bIsFlip)
	{
		if (true == bIsLeft)
		{
			Offset.X *= -1.0f;
		}
		else
		{
			Offset.X *= -1.0f;
		}
	}
	else
	{
		if (true == bIsLeft)
		{
			Offset.X *= -1.0f;
		}
		else
		{
			Offset.X *= 1.0f;
		}
	}

	BodyRenderer->SetRelativeLocation({ Offset.X + RendererOffset.X, Offset.Y + RendererOffset.Y, ZSort });
}

void AMonster::SetRendererOffset(UContentsRenderer* _Renderer, FVector _Offset)
{
	FVector Offset = _Offset;
	if (true == bIsFlip)
	{
		if (true == bIsLeft)
		{
			Offset.X *= -1.0f;
		}
		else
		{
			Offset.X *= -1.0f;
		}
	}
	else
	{
		if (true == bIsLeft)
		{
			Offset.X *= -1.0f;
		}
		else
		{
			Offset.X *= 1.0f;
		}
	}

	_Renderer->SetRelativeLocation({ Offset.X + RendererOffset.X, Offset.Y + RendererOffset.Y, ZSort });
}

void AMonster::SetCollisionOffset(UCollision* _Target, FVector _Offset)
{
	FVector Offset = _Offset;
	if (true == bIsFlip)
	{
		if (true == bIsLeft)
		{
			Offset.X *= -1.0f;
		}
		else
		{
			Offset.X *= -1.0f;
		}
	}
	else
	{
		if (true == bIsLeft)
		{
			Offset.X *= -1.0f;
		}
		else
		{
			Offset.X *= 1.0f;
		}
	}

	_Target->SetRelativeLocation({ Offset.X + BodyCollisionOffset.X, Offset.Y + BodyCollisionOffset.Y, ZSort });
}

void AMonster::SetWalkRendererOffset()
{
	if (FVector::ZERO == WalkRendererOffset)
	{
		return;
	}
	BodyRenderer->SetRelativeLocation({ WalkRendererOffset.X, WalkRendererOffset.Y, ZSort });
}

void AMonster::SetAttackRendererOffset()
{
	if (FVector::ZERO == AttackRendererOffset)
	{
		return;
	}
	BodyRenderer->SetRelativeLocation({ AttackRendererOffset.X, AttackRendererOffset.Y, ZSort });
}

void AMonster::CreateDetectCollision()
{
	DetectCollision = CreateDefaultSubObject<UCollision>();
	DetectCollision->SetupAttachment(RootComponent);
	DetectCollision->SetScale3D(DetectRange);
	DetectCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	DetectCollision->SetCollisionProfileName("MonsterDetect");
	DetectCollision->SetDebugColor({ 1.0f, 1.0f, 0.0f });
}

void AMonster::CreateCenterPoint()
{
	CenterPoint = CreateDefaultSubObject<UCollision>();
	CenterPoint->SetupAttachment(RootComponent);
	CenterPoint->SetScale3D({ 10, 10 });
	CenterPoint->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	CenterPoint->SetCollisionProfileName("MonsterPoint");
	CenterPoint->SetDebugColor({ 1.0f, 0.0f, 0.0f });
}

void AMonster::CreatePixelCollision()
{
	PixelCollision = CreateDefaultSubObject<UCollision>();
	PixelCollision->SetupAttachment(RootComponent);
	PixelCollision->SetCollisionProfileName("MonsterPoint");
	PixelCollision->SetScale3D({ CollisionScale.X, 5.0f });
	float Offset = ::abs(BodyCollision->GetWorldScale3D().Half().Y);
	PixelCollision->SetRelativeLocation({ 0.0f + BodyCollisionOffset.X, -Offset + BodyCollisionOffset.Y});
	PixelCollision->SetDebugColor({ 1.0f, 1.0f, 1.0f });
}

void AMonster::ActivePixelCollision()
{
	if (true == IsCurRoom())
	{
		ParentRoom->CheckPixelCollisionWithWall(this, Stat.GetVelocity(), bIsLeft, WallPointOffest);
		ParentRoom->CheckPixelCollisionWithCeil(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft, CeilPointOffset);
	}
}

void AMonster::ActiveGravity()
{
	if (true == IsCurRoom())
	{
		ParentRoom->CheckPixelCollisionWithGravity(this, GravityPointOffset);
	}
}

float AMonster::GetToPlayerDistance()
{
	FVector MonsterPos = GetActorLocation();
	FVector KnightPos = AKnight::GetPawn()->GetActorLocation();
	float Distance = ::abs(MonsterPos.Length() - KnightPos.Length());
	return Distance;
}

void AMonster::PlayStaticSound(std::string_view _SoundFileName, float _MaxVolume , float _MaxDistance)
{
	bIsSoundStop = false;
	bIsInitSound = false;

	if ("" == _SoundFileName)
	{
		return;
	}
	float Distance = GetToPlayerDistance();
	if (_MaxDistance > Distance)
	{
		float Volume = UEngineMath::ClampMin(100.0f / Distance, _MaxVolume);
		Sound.SetVolume(Volume);
		if (false == Sound.IsPlaying())
		{
			StaticSound = _SoundFileName;

			Sound.Stop();
			Sound = UEngineSound::Play(StaticSound);
		}
	}
}

void AMonster::SoundPlay(std::string_view _SoundFileName, bool _IsLoop, bool _PrevSoundStop, float _Volume)
{
	if ("" == _SoundFileName)
	{
		return;
	}
	if (true == bIsInitSound)
	{
		return;
	}
	if (true == _PrevSoundStop)
	{
		if (false == bIsSoundStop)
		{
			Sound.Stop();
			bIsSoundStop = true;
		}
	}
	if (true == _IsLoop)
	{
		if (false == Sound.IsPlaying())
		{
			Sound = UEngineSound::Play(_SoundFileName);
			Sound.SetVolume(_Volume);

			Sound.Loop(999);
		}
	}
	else
	{
		Sound = UEngineSound::Play(_SoundFileName);
		Sound.SetVolume(_Volume);
		bIsInitSound = true;
	}




}

