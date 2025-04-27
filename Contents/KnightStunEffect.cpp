#include "PreCompile.h"
#include "KnightStunEffect.h"
#include <EngineBase/EngineRandom.h>
#include "Particle.h"

KnightStunEffect::KnightStunEffect()
{
	SetName("Knight Stun Effect");

	float StunInitTime = 0.4f;
	float StunFrame = 0.05f;

	std::string StunEffect01 = "StunEffect01";
	BodyRenderer->CreateAnimation(StunEffect01, StunEffect01, { 0, 1, 2, 3 }, { StunInitTime , StunFrame ,StunFrame ,StunFrame }, false);

	SetZSort(static_cast<int>(EZOrder::KNIGHT_STUN_EFFECT));
	BodyRenderer->ChangeAnimation(StunEffect01);

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

KnightStunEffect::~KnightStunEffect()
{
}

void KnightStunEffect::BeginPlay()
{
	AEffect::BeginPlay();
}

void KnightStunEffect::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);
}

void KnightStunEffect::CreateStunImpactEffect()
{
	UEngineDebug::OutPutString("Spawn Stun Impact Effect");
	UEngineRandom Random;
	float Degree0 = Random.Randomfloat(0.0f, 360.0f);
	float Scale0 = Random.Randomfloat(0.9f, 1.2f);
	FVector Rotation = FVector::ZERO;
	Rotation.Z = Degree0;

	AKnightEffect* BlackEffect = GetWorld()->SpawnActor<AKnightEffect>().get();
	BlackEffect->SetName("Stun Black Effect");
	AKnight* Knight = AKnight::GetPawn();
	BlackEffect->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FRONT) - 1);
	BlackEffect->ChangeAnimation("StunEffect02", Knight->GetActorLocation());
	BlackEffect->SetScale(Scale0);
	BlackEffect->GetRenderer()->SetMulColor({ 0.5f, 0.5f, 0.5f }, 1.0f);
	BlackEffect->SetLocation(Knight, FVector::ZERO, Rotation);


	AParticle* BlackParticle = GetWorld()->SpawnActor<AParticle>().get();
	BlackParticle->CreateParticle("BlackParticle", 30, 0.01f, Knight->GetActorLocation());
	BlackParticle->SetAlpha(1.0f);
	BlackParticle->SetRandomScale(0.5f, 1.0f);
	BlackParticle->SetDecayScale(true, 0.6f);
	BlackParticle->SetParticleOption(EParticleType::RANDOM, -600.0f, 600.0f);


	Random.SetSeed(reinterpret_cast<long long>(this));
	float Degree1 = Random.Randomfloat(0.0f, 360.0f);
	float Scale1 = Random.Randomfloat(1.0f, 1.2f);
	Rotation.Z = Degree1;

	AKnightEffect* WhiteEffect = GetWorld()->SpawnActor<AKnightEffect>().get();
	WhiteEffect->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FRONT) - 2);
	WhiteEffect->SetName("Stun White Effect");
	WhiteEffect->ChangeAnimation("StunEffect03", Knight->GetActorLocation());
	WhiteEffect->SetScale(Scale1);
	WhiteEffect->SetLocation(Knight, FVector::ZERO, Rotation);

	AParticle* WhiteParticle = GetWorld()->SpawnActor<AParticle>().get();
	WhiteParticle->CreateParticle("WhiteParticle", 10, 0.01f, Knight->GetActorLocation());
	WhiteParticle->SetAlpha(1.0f);
	WhiteParticle->SetRandomScale(0.5f, 1.0f);
	WhiteParticle->SetDecayScale(true, 0.6f);
	WhiteParticle->SetParticleOption(EParticleType::RANDOM, -300.0f, 300.0f);
}


