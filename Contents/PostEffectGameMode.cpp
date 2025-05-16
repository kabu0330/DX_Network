#include "PreCompile.h"
#include "PostEffectGameMode.h"
#include <EngineCore/EngineRenderTarget.h>
#include "TestPostEffect.h"
#include <EngineCore/EngineCamera.h>
#include "BasicVisualActor.h"
#include <EngineCore/PlayerController.h>
#include <EnginePlatform/EngineInput.h>

APostEffectGameMode::APostEffectGameMode()
{
	Camera = GetWorld()->GetMainCamera().get();
	Camera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("JHJEffect");
		Mat->SetVertexShader("JHJEffect.fx");
		Mat->SetPixelShader("JHJEffect.fx");
		// ���� ���õ� ���� ������ ���Ƽ� ������ �ټ� �����Ƿ�
		Mat->SetDepthStencilState("ALWAYS");

	}

	ABasicVisualActor* TestActor = GetWorld()->SpawnActor<ABasicVisualActor>().get();
	TestActor->SetActorLocation(FVector(0, 0, 0));

	APlayerController* PlayerController = GetWorld()->SpawnActor<APlayerController>().get();
	PlayerController->Possess(TestActor);
	PlayerController->SwitchArrowKeys();
}

APostEffectGameMode::~APostEffectGameMode()
{
}

void APostEffectGameMode::BeginPlay()
{
	AGameMode::BeginPlay();

	UEngineRenderTarget* LastTarget = GetWorld()->GetLastRenderTarget();
	LastTarget->AddEffet<UTestPostEffect>();

	std::shared_ptr<UPostEffect> Effect = LastTarget->GetPostEffect(0);
	Effect->IsActive = false;
}

void APostEffectGameMode::Tick(float _DeltaTime)
{
	AGameMode::Tick(_DeltaTime);

	if (UEngineInput::IsDown('K'))
	{
		UEngineRenderTarget* LastTarget = GetWorld()->GetLastRenderTarget();

		// ����Ʈ ����Ʈ �Ѱ� ����.
		std::shared_ptr<UPostEffect> Effect = LastTarget->GetPostEffect(0);
		Effect->IsActive = true;
	}
}

