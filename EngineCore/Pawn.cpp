#include "PreCompile.h"
#include "Pawn.h"
#include "DefaultSceneComponent.h"
#include "SpriteRenderer.h"

APawn::APawn()
{
	std::shared_ptr<UDefaultSceneComponent> SceneComponent = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = SceneComponent;
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>().get();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetScale3D(FVector(50, 50, 50));
}

APawn::~APawn()
{
}

void APawn::BeginPlay()
{
	Super::BeginPlay();
}

void APawn::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

