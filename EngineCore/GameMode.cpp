#include "PreCompile.h"
#include "GameMode.h"
#include "CameraActor.h"
#include "EngineCamera.h"
#include "PlayerController.h"

AGameMode::AGameMode()
{
	MainCamera = GetWorld()->GetMainCamera().get();
	MainCamera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	MainCamera->GetCameraComponent()->SetZSort(0, true);

	PlayerController = GetWorld()->SpawnActor<APlayerController>().get();
	PlayerController->SwitchArrowKeys(); // ¹æÇâÅ°
}

AGameMode::~AGameMode()
{
}

void AGameMode::BeginPlay()
{
	Super::BeginPlay();

	APawn* Pawn = GetWorld()->GetMainPawn();
	PlayerController->Possess(Pawn);
}

