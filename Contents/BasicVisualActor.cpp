#include "PreCompile.h"
#include "BasicVisualActor.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>

ABasicVisualActor::ABasicVisualActor()
{
	std::shared_ptr<UDefaultSceneComponent> SceneComponent = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = SceneComponent;
	Renderer = CreateDefaultSubObject<USpriteRenderer>().get();
	Renderer->SetupAttachment(RootComponent);
	Renderer->SetScale3D(FVector(50, 50, 50));
}

ABasicVisualActor::~ABasicVisualActor()
{
}

void ABasicVisualActor::BeginPlay()
{
	Super::BeginPlay();

}

void ABasicVisualActor::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

