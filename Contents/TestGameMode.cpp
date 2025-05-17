#include "PreCompile.h"
#include "TestGameMode.h"
#include "BasicVisualActor.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/PlayerController.h>

ATestGameMode::ATestGameMode()
{


}

ATestGameMode::~ATestGameMode()
{
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ABasicVisualActor* TestActor = GetWorld()->SpawnActor<ABasicVisualActor>().get();
	TestActor->SetActorLocation(FVector(0, 0, 0));

	APlayerController* PlayerController = GetWorld()->SpawnActor<APlayerController>().get();
	PlayerController->Possess(TestActor);
	PlayerController->SwitchArrowKeys();
}

void ATestGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

}

void ATestGameMode::LevelChangeStart()
{

}

