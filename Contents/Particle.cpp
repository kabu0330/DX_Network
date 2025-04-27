#include "PreCompile.h"
#include "Particle.h"
#include <EngineBase/EngineRandom.h>
#include "Effect.h"

AParticle::AParticle()
{
	SetName("AParticle");
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
}

AParticle::~AParticle()
{
}

void AParticle::SetParticleOption(EParticleType _ParticleType, float _MinVelocity, float _MaxVelocity, FVector _Dir, FVector _Force)
{
	Force = _Force;
	ParticleType = _ParticleType;
	switch (ParticleType)
	{
	case EParticleType::REVERSE:
	{
		if (FVector::ZERO == _Dir)
		{
			MSGASSERT("��ƼŬ Ÿ���� REVERSE�� ���� ������ �ʼ� �Է� �Ű������Դϴ�.");
			return;
		}

		_Dir.Normalize();

		for (int i = 0; i < MaxParticles; i++)
		{
			// ���� ��ġ
			UEngineRandom Random;
			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + i);
			float RandomSpeed = Random.Randomfloat(_MinVelocity, _MaxVelocity);

			Velocities[i] = _Dir * RandomSpeed;
		}
	}
		break;
	case EParticleType::RISING:
	{
		for (int i = 0; i < MaxParticles; i++)
		{
			UEngineRandom Random;
			long long Seed = reinterpret_cast<long long>(this) + std::chrono::high_resolution_clock::now().time_since_epoch().count() + i;
			Random.SetSeed(Seed);
			float RandomSpeed = Random.Randomfloat(50.0f, _MaxVelocity);
			float RandomPosX = Random.Randomfloat(-0.02f, 0.02f);
			FVector RisingVector = FVector(FVector::UP.X + RandomPosX, FVector::UP.Y) * RandomSpeed;
			Velocities[i] = RisingVector;
		}
		break;
	}	
	case EParticleType::RANDOM:
	case EParticleType::MAX_PARTICLE:
	{
		for (int i = 0; i < MaxParticles; i++)
		{
			// ���� ��ġ
			UEngineRandom Random;
			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + i);
			float RandomSpeedX = Random.Randomfloat(_MinVelocity, _MaxVelocity);

			Random.SetSeed(reinterpret_cast<long long>(this) + time(nullptr) + (i * 2)); // �ѹ� �� ����
			float RandomSpeedY = Random.Randomfloat(_MinVelocity, _MaxVelocity);

			Velocities[i] = FVector(RandomSpeedX, RandomSpeedY);
		}
	}
		break;
	default:
		break;
	}
}

void AParticle::SpawnParticle()
{
	int RemainParticles = MaxParticles - Count;

	if (0 >= RemainParticles) 
	{
		return; // �ִ� ���� ���� �ʰ�
	}

	for (int i = 0; i < RemainParticles; i++)
	{
		UEngineRandom Random;
		long long Seed = reinterpret_cast<long long>(this) + std::chrono::high_resolution_clock::now().time_since_epoch().count() + i;
		Random.SetSeed(Seed);
		float RandomScale = Random.Randomfloat(RandomScaleMin, RandomScaleMax);

		std::shared_ptr<AEffect> Effect = GetWorld()->SpawnActor<AEffect>();
		Effect->ChangeAnimation(EffectName);

		SetEmitterPosition(RandomScale); // Ư�� ��ƼŬ �ɼ��� ��, EmitterPosition ���� ��ȯ
		EffectInitPos = EmitterPosition;
		Effect->SetLocation(EmitterPosition);

		Effect->SetZSort(EZOrder::HitParticleEffect);
		Effect->SetScale(RandomScale);
		Effect->GetRenderer()->SetAlpha(Alpha);

		Effect->SetZSort(static_cast<int>(EZOrder::HitParticleEffect) - ( i + static_cast<int>(ZOrderOffset + ZOrder)));

		FParticleData NewParticle(Effect, Velocities[i]);

		Particles.push_back(NewParticle);
		++Count;
		++ZOrder;
	}
}

void AParticle::Update(float _DeltaTime)
{
	for (auto& Particle : Particles)
	{
		Particle.Velocity += Force * _DeltaTime;
		Particle.Position += Particle.Velocity * _DeltaTime;

		if (nullptr != Particle.Effect && true == Particle.Effect->bIsValid)
		{
			Particle.Effect->SetLocation(EffectInitPos + Particle.Position);

			// �ɼ� ����
			// 1. ũ�� ���� �۰�
			if (true == ScaleDecay)
			{
				Particle.Effect->SetScaleFadeOut(ScaleDecayRatio);
			}
		}
	}
}

void AParticle::Remove()
{	
	Particles.erase(
		std::remove_if(Particles.begin(), Particles.end(),
			[](const FParticleData& Particle) {
				// Effect�� ��ȿ���� �ʰų� LifeTime�� ���� ��� ����
				return Particle.Effect->GetRenderer() == nullptr || true == Particle.Effect->GetRenderer()->IsCurAnimationEnd() || 0.1f >= Particle.Effect->GetRenderer()->GetAutoScaleRatio();
			}),
		Particles.end());
	if (0 >= Particles.size())
	{
		UEngineDebug::OutPutString("��ƼŬ �Ҹ�");
		Destroy();
	}
}

void AParticle::BeginPlay()
{
	AActor::BeginPlay();
}

void AParticle::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	SpawnParticle();
	Update(_DeltaTime);
	Remove();
	ResetZOrder();

}

void AParticle::SetEmitterPosition(float _RandomValue)
{
	switch (ParticleType)
	{
	case EParticleType::RISING:
	{
		// Y���� �׻� ȭ�� �Ʒ�, X���� ȭ�� �ʺ��� ����
		EmitterPosition.Y = -UEngineCore::GetScreenScale().Half().Y;

		float ScreenHalfSizeX = UEngineCore::GetScreenScale().Half().X;

		std::random_device rd;
		std::mt19937_64 RandomGen(rd() + static_cast<__int64>(_RandomValue));
		std::uniform_real_distribution<float> Dist(-ScreenHalfSizeX, ScreenHalfSizeX);

		EmitterPosition.X = Dist(RandomGen);
	}
		break;
	case EParticleType::RANDOM:
		break;
	case EParticleType::REVERSE:
		break;
	case EParticleType::MAX_PARTICLE:
		break;
	default:
		break;
	}
}

void AParticle::ResetZOrder()
{
	if (ZOrder > 200.0f)
	{
		ZOrder = 0.0f;
	}
}

