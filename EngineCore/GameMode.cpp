#include "PreCompile.h"
#include "GameMode.h"
#include "CameraActor.h"
#include "EngineCamera.h"

AGameMode::AGameMode()
{
	MainCamera = GetWorld()->GetMainCamera().get();
	MainCamera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	MainCamera->GetCameraComponent()->SetZSort(0, true);
}

AGameMode::~AGameMode()
{
}

