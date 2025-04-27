#include "PreCompile.h"
#include "TestGameMode.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineCore.h>
#include "CollisionManager.h"
#include "PlayHUD.h"

ATestGameMode::ATestGameMode()
{
	ACameraActor* Camera = GetWorld()->GetMainCamera().get();
	Camera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);

	UCollisionManager::CreateCollisionProfile(this);
	UCollisionManager::LinkCollision(this);
}

ATestGameMode::~ATestGameMode()
{
}

void ATestGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void ATestGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	if (UEngineInput::IsDown(VK_F4))
	{
		UEngineCore::ResetLevel<ATestGameMode, AKnight, APlayHUD>("TestMode");
	}
}

