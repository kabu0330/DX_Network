#include "PreCompile.h"
#include "PlayGameMode.h"
#include <EnginePlatform/EngineWinImage.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/TimeEventComponent.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include <EngineCore/EngineCore.h>
#include "Room.h"
#include "CollisionManager.h"
#include "RoomManager.h"
#include "DebugWindowGUI.h"
#include "PlayHUD.h"


FVector APlayGameMode::MousePos = { 0.0f, 0.0f, 0.0f };
FVector APlayGameMode::KnightPos = { 0.0f, 0.0f, 0.0f };

APlayGameMode::APlayGameMode()
{
#ifdef _DEBUG
	UCollision::GetDebugModeRef() = true;
#else
	UCollision::GetDebugModeRef() = false;
#endif // _DEBUG

	PlayGameMode = this;
	SetCamera();
	CreateAndLinkCollisionGroup();

	Rooms.CreateRoom(this);
}

void APlayGameMode::SetCamera()
{
	Camera = GetWorld()->GetMainCamera().get();
	Camera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
	//Camera->GetCameraComponent()->SetProjectionType(EProjectionType::Perspective);
}

void APlayGameMode::CreateAndLinkCollisionGroup()
{
	UCollisionManager::CreateCollisionProfile(this);
	UCollisionManager::LinkCollision(this);
}

void APlayGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	CheckInfo();
	CheckDebugInput();
	SetActiveRoom();
}

void APlayGameMode::CheckDebugInput()
{
	if (UEngineInput::IsDown('T'))
	{
		ARoom::SwitchDebugActiveGravity();
	}
	if (UEngineInput::IsDown('P'))
	{
		GetWorld()->GetMainCamera()->FreeCameraSwitch();
	}
	if (UEngineInput::IsDown(VK_F4))
	{
		UEngineCore::ResetLevel<APlayGameMode, AKnight, APlayHUD>("Play");
	}
}

void APlayGameMode::CheckInfo()
{
	MousePos = Camera->ScreenMousePosToWorldPos();
	KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
}

void APlayGameMode::SetActiveRoom()
{
	ARoom* PrevRoom = Rooms.GetPrevRoom();
	ARoom* CurRoom = ARoom::GetCurRoom();
	if (PrevRoom != CurRoom)
	{
		std::vector<std::shared_ptr<ARoom>> VectorRooms = Rooms.GetRoomsRef();
		for (std::shared_ptr<ARoom> Room : VectorRooms)
		{
			if (Room.get() == CurRoom)
			{
				Room->SetRoomActive(true);
			}
			else
			{
				Room->SetRoomActive(false);
			}
		}
	}
}

void APlayGameMode::ShowPrompt()
{
	float Time = 1.0f;
	float Gap = 1.5f;
	float ShowGap = 4.0f;
	
	Time += Gap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveJumpPrompt();
		});
	Time += ShowGap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveFalsePrompt();
		});
	Time += Gap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveDashPrompt();
		});
	Time += ShowGap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveFalsePrompt();
		});
	Time += Gap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveSlashPrompt();
		});
	Time += ShowGap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveFalsePrompt();
		});
	Time += Gap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveFireballPrompt();
		});
	Time += ShowGap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveFalsePrompt();
		});
	Time += Gap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveFocusPrompt();
		});
	Time += ShowGap;
	TimeEventer->AddEndEvent(Time, []()
		{
			APlayHUD::GetHUD()->ActiveFalsePrompt();
		});
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
	SetBasePoint(); // 원점 0, 0 표기
	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>();
	ShowPrompt();
}

void APlayGameMode::SetBasePoint()
{ 
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BasePointCollision = CreateDefaultSubObject<UCollision>();
	BasePointCollision->SetupAttachment(RootComponent);
	BasePointCollision->SetCollisionProfileName("Door");
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);
	BasePointCollision->GetTransformRef().Location.Z = ZSort;
	BasePointCollision->SetScale3D({8, 8});
	BasePointCollision->SetDebugColor({1.0f, 0.0f, 0.0f, 1.0f });
}

void APlayGameMode::LevelChangeStart()
{
	UEngineSound::AllSoundOff();
	Sound = UEngineSound::Play("Dirtmouth 1.wav");
	APlayHUD* HUD = dynamic_cast<APlayHUD*>(GetWorld()->GetHUD());
	HUD->FadeOut(3.0f, 0.5f);

	Window = UEngineGUI::FindGUIWindow<UDebugWindowGUI>("DebugWindow");

	if (nullptr == Window)
	{
		Window = UEngineGUI::CreateGUIWindow<UDebugWindowGUI>("DebugWindow");
	}

	Window->SetActive(true);
}

void APlayGameMode::LevelChangeEnd()
{
	
}

APlayGameMode::~APlayGameMode()
{
}