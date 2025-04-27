#include "PreCompile.h"
#include "MapEditorGameMode.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/Actor.h>

#include "ContentsEditorGUI.h"
#include "MapObject.h"
#include "Background.h"
#include "DebugWindowGUI.h"
#include "MapEditorGUI.h"
#include "CollisionManager.h"

std::shared_ptr<AMapObject> AMapEditorGameMode::MapObject = nullptr;


AMapEditorGameMode::AMapEditorGameMode()
{
	// 충돌은 레벨마다 공유
	UCollisionManager::CreateCollisionProfile(this);
	UCollisionManager::LinkCollision(this);

	Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
	FVector ScreenSize = UEngineCore::GetScreenScale();
	Camera->SetActorLocation({ScreenSize.X * 0.5f, -ScreenSize.Y * 0.5f });
	//Camera->GetCameraComponent()->SetProjectionType(EProjectionType::Perspective);

	UEngineGUI::CreateGUIWindow<MapEditorGUI>("MapEditorWindow");

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	//BackTexture = CreateDefaultSubObject<USpriteRenderer>();
	//BackTexture->SetupAttachment(RootComponent);
	//float ZSort = 3999.0f;
	//BackTexture->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	//BackTexture->SetTexture("dartmount_sample.bmp", true);
	//BackTexture->ColorData.MulColor *= {1.0f, 1.0f, 1.0f, 0.0f};
}

AMapEditorGameMode::~AMapEditorGameMode()
{
}

void AMapEditorGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	CheckInput(_DeltaTime);
}

void AMapEditorGameMode::CheckInput(float _DeltaTime)
{
	float Speed = 700.0f;
	if (UEngineInput::IsPress('A'))
	{
		Camera->AddActorLocation({ - Speed * _DeltaTime, 0.0f, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Camera->AddActorLocation({ Speed * _DeltaTime, 0.0f, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('W'))
	{
		Camera->AddActorLocation({ 0.0f , Speed * _DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		Camera->AddActorLocation({ 0.0f , -Speed * _DeltaTime, 0.0f, 0.0f });
	}

	// 미리보기 취소
	if (true == UEngineInput::IsDown('~'))
	{
		MapEditorWindow->IsPreviewRef() = false;

	}
}

void AMapEditorGameMode::LevelChangeStart()
{
	UEngineGUI::AllWindowOff();

	{
		std::shared_ptr<UContentsEditorGUI> Window = UEngineGUI::FindGUIWindow<UContentsEditorGUI>("ContentsEditorGUI");

		if (nullptr == Window)
		{
			Window = UEngineGUI::CreateGUIWindow<UContentsEditorGUI>("ContentsEditorGUI");
		}

		Window->SetActive(true);
	}

	{
		MapEditorWindow = UEngineGUI::FindGUIWindow<MapEditorGUI>("MapEditorWindow");

		if (nullptr == MapEditorWindow)
		{
			MapEditorWindow = UEngineGUI::CreateGUIWindow<MapEditorGUI>("MapEditorWindow");
		}

		MapEditorWindow->SetActive(true);
	}

	{
		std::shared_ptr<UDebugWindowGUI> Window = UEngineGUI::FindGUIWindow<UDebugWindowGUI>("DebugWindow");

		if (nullptr == Window)
		{
			Window = UEngineGUI::CreateGUIWindow<UDebugWindowGUI>("DebugWindow");
		}

		Window->SetActive(true);
	}
}