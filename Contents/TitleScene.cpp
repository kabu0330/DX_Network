#include "PreCompile.h"
#include "TitleScene.h"
#include <EngineCore/EngineCore.h>
#include "Particle.h"

ATitleScene::ATitleScene()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BackgroundRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
	BackgroundRenderer->SetTexture("main.bmp", true, 0.67f);
	BackgroundRenderer->SetRelativeLocation({ 0.0f, 0.0f, 1000.0f });

	TitleLogo = CreateDefaultSubObject<UContentsRenderer>();
	TitleLogo->SetupAttachment(RootComponent);
	TitleLogo->SetTexture("TitleLogo.png", true, 0.6f);
	FVector ScreenSize = UEngineCore::GetScreenScale();
	TitleLogo->SetRelativeLocation({ 0.0f, ScreenSize.Y * 0.25f , 999.0f });

	CreditsLogo = CreateDefaultSubObject<UContentsRenderer>();
	CreditsLogo->SetupAttachment(RootComponent);
	CreditsLogo->SetTexture("creditslogo.png", true, 0.4f);
	CreditsLogo->SetRelativeLocation({ ScreenSize.X * 0.3f, ScreenSize.Y * -0.27f , 999.0f });

	StartLogo = CreateDefaultSubObject<UContentsRenderer>();
	StartLogo->SetupAttachment(RootComponent);
	StartLogo->SetTexture("Start.png", true, 0.6f);
	StartLogo->SetRelativeLocation({ 0.0f, ScreenSize.Y * -0.18f , 999.0f });
}

ATitleScene::~ATitleScene()
{
}

void ATitleScene::BeginPlay()
{
	AActor::BeginPlay();
}

void ATitleScene::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	TimeElapsed += _DeltaTime;
	if (TimeElapsed >= Cooldown)
	{
		TimeElapsed = 0.0f;
		CreateBlackParticle(_DeltaTime);
	}
}

void ATitleScene::CreateBlackParticle(float _DeltaTime)
{
	AParticle* BlackParticle = GetWorld()->SpawnActor<AParticle>().get();

	BlackParticle->CreateParticle("BlackParticle", 1, 0.01f, FVector{ 0.0f, 0.0f, -1000.0f });
	BlackParticle->SetAlpha(0.7f);

	BlackParticle->SetRandomScale(0.5f, 1.5f);
	BlackParticle->SetDecayScale(true, 0.3f);
	BlackParticle->SetParticleOption(EParticleType::RISING, 0.0f, 100.0f);
}

