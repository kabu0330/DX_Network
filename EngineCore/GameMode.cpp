#include "PreCompile.h"
#include "GameMode.h"
#include "CameraActor.h"
#include "EngineCamera.h"
#include "PlayerController.h"
#include <EnginePlatform/EngineIOCPServer.h>
#include <EnginePlatform/EngineListenServer.h>

AGameMode::AGameMode()
{
	MainCamera = GetWorld()->GetMainCamera();
	MainCamera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	MainCamera->GetCameraComponent()->SetZSort(0, true);

	PlayerController = GetWorld()->SpawnActor<APlayerController>();
	PlayerController->SwitchArrowKeys(); // 방향키로 이동 전환
}

void AGameMode::BeginPlay()
{
	Super::BeginPlay();

	APawn* Pawn = GetWorld()->GetMainPawn();
	PlayerController->Possess(Pawn);
}

void AGameMode::StartServer(int _Port)
{
	std::shared_ptr<UEngineListenServer> Server = std::make_shared<UEngineListenServer>();
	NetworkInstance = Server;
	Server->OpenServer(_Port);
	Server->SetProtocolFunction([this](std::shared_ptr<UEngineProtocol> _Protocol)
		{
			GetWorld()->AddProtocol(_Protocol);
		});
}

void AGameMode::StartIOCPServer(int _Port)
{
	std::shared_ptr<UEngineIOCPServer> Server = std::make_shared<UEngineIOCPServer>();
	NetworkInstance = Server;
	Server->OpenIOCPServer(_Port);
	Server->SetProtocolFunction([this](std::shared_ptr<UEngineProtocol> _Protocol)
		{
			GetWorld()->AddProtocol(_Protocol);
		});
}

void AGameMode::JoinServer(std::string_view _IP, int _Port)
{
	std::shared_ptr<UEngineClient> Client = std::make_shared<UEngineClient>();
	NetworkInstance = Client;
	Client->Connect(_IP, _Port);
	Client->SetProtocolFunction([this](std::shared_ptr<UEngineProtocol> _Protocol)
		{
			GetWorld()->AddProtocol(_Protocol);
		});
}

AGameMode::~AGameMode()
{
	if (nullptr != NetworkInstance)
	{
		NetworkInstance->Release();
	}
	NetworkInstance = nullptr;
}

