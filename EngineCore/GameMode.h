#pragma once
#include "Actor.h"
#include <EnginePlatform/EngineNetwork.h>
#include <EnginePlatform/EngineServer.h>
#include <EnginePlatform/EngineClient.h>

class APlayerController;
class ACameraActor;



// 설명 : 게임 규칙, 네트워크
class AGameMode : public AActor
{
public:
	ENGINEAPI AGameMode();
	ENGINEAPI ~AGameMode();

	ENGINEAPI std::shared_ptr<ACameraActor> GetMainCamera()
	{
		return MainCamera;
	}

	ENGINEAPI std::shared_ptr<UEngineNetwork> GetNetwork()
	{
		return NetworkInstance;
	}
	
	std::shared_ptr<UEngineServer> GetServer()
	{
		return std::dynamic_pointer_cast<UEngineServer>(NetworkInstance);
	}
	std::shared_ptr<UEngineClient> GetClient()
	{
		return std::dynamic_pointer_cast<UEngineClient>(NetworkInstance);
	}
	
	ENGINEAPI void StartServer(int _Port);
	ENGINEAPI void JoinServer(std::string_view _IP, int _Port);

protected:
	ENGINEAPI void BeginPlay() override; // 게임모드 생성 즉시 Test Pawn을 생성하고 싶으면 호출

	std::shared_ptr<APlayerController> PlayerController = nullptr;
	std::shared_ptr<ACameraActor> MainCamera = nullptr;

private:

	std::shared_ptr<UEngineNetwork> NetworkInstance = nullptr;

private:
	// delete Function
	AGameMode(const AGameMode& _Other) = delete;
	AGameMode(AGameMode&& _Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& _Other) = delete;
	AGameMode& operator=(AGameMode&& _Other) noexcept = delete;
};

