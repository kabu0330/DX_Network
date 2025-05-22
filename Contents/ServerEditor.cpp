#include "PreCompile.h"
#include "ServerEditor.h"
#include "ServerPawn.h"
#include <EnginePlatform/EngineDispatcher.h>
#include "ContentPacket.h"
#include "ServerGameMode.h"
#include <EngineCore/PlayerController.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineCore/EngineCore.h>

void UServerEditor::OnGUI(float _DeltaTime)
{
	if (nullptr == GetWorld()->GetGameMode()->GetNetwork())
	{
		ImGui::InputInt("Port", &Port);
		if (true == ImGui::Button(UEngineString::AnsiToUTF8("방만들기").c_str()))
		{
			GetWorld()->GetGameMode()->StartServer(Port);
			Server = GetWorld()->GetGameMode()->GetServer<UEngineServer>();
			CreateServer(Server);
			GEngine->GetMainWindow().SetWindowTitle("Server");
		}

		if (true == ImGui::Button(UEngineString::AnsiToUTF8("IOCP방만들기").c_str()))
		{
			GetWorld()->GetGameMode()->StartIOCPServer(Port);
			IOCPServer = GetWorld()->GetGameMode()->GetServer<UEngineIOCPServer>();
			CreateServer(IOCPServer);
			GEngine->GetMainWindow().SetWindowTitle("IOCP Server");
		}

		IP.resize(256);
		ImGui::InputText("IP", &IP[0], IP.size());
		if (true == ImGui::Button(UEngineString::AnsiToUTF8("같이하기").c_str()))
		{
			GetWorld()->GetGameMode()->JoinServer(IP, Port);
			std::shared_ptr<UEngineClient> Client = GetWorld()->GetGameMode()->GetClient();
			Connect(Client);
			GEngine->GetMainWindow().SetWindowTitle("Client");
		}
	}
	else
	{
		std::shared_ptr<UEngineNetwork> Net = GetWorld()->GetGameMode()->GetNetwork();
		if (nullptr != std::dynamic_pointer_cast<UEngineClient>(Net))
		{
			ImGui::Text("Client");
		}
		else if (nullptr != std::dynamic_pointer_cast<UEngineServer>(Net))
		{
			ImGui::Text("Server");
		}
		else if (nullptr != std::dynamic_pointer_cast<UEngineIOCPServer>(Net))
		{
			ImGui::Text("IOCP Server");
		}
	}
}

void UServerEditor::CreateServer(std::shared_ptr<UEngineServer> _Net)
{
	AServerPawn* Pawn = GetWorld()->GetMainPawn<AServerPawn>();
	int ObjectToken = _Net->CreateSessionToken();
	Pawn->InitNetObject(ObjectToken, _Net->GetSessionToken());

	_Net->GetDispatcher().AddHandler<UObjectUpdatePacket>(static_cast<int>(EContentsPacketType::OBJECT_UPDATE), 
		[this, _Net](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			AServerPawn* ServerPawn = UNetObject::GetConvertNetObject<AServerPawn>(Token);
			if (false == UNetObject::IsNetObject(Token))
			{
				std::shared_ptr<AServerPawn> NewServerPawn = GetWorld()->SpawnActor<AServerPawn>();
				ServerPawn = NewServerPawn.get();
				ServerPawn->SetControllOff();

				//AServerGameMode* GameMode = ServerPawn->GetWorld()->GetGameMode<AServerGameMode>();
				//APlayerController* PlayerController = GameMode->GetPlayerController();
				//PlayerController->Possess(ServerPawn);
				
				ServerPawn->InitNetObject(_Packet->GetObjectToken(), _Packet->GetSessionToken());
			}

			ServerPawn->SetActorLocation(_Packet->GetPosition());
			_Net->SendPacket(_Packet);
		});
}

void UServerEditor::CreateNetObject(std::shared_ptr<UUserAccessPacket> _Packet)
{
	UserAccessPacket = _Packet;
	AServerPawn* Pawn = GetWorld()->GetMainPawn<AServerPawn>();
	Pawn->InitNetObject(UserAccessPacket->GetObjectToken(), UserAccessPacket->GetSessionToken());
}

void UServerEditor::Connect(std::shared_ptr<UEngineClient> _Net)
{
	_Net->SetUserAccessFunction(std::bind(&UServerEditor::CreateNetObject, this, std::placeholders::_1));

	_Net->GetDispatcher().AddHandler<UObjectUpdatePacket>(static_cast<int>(EContentsPacketType::OBJECT_UPDATE), 
		[this](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			AServerPawn* ServerPawn = UNetObject::GetConvertNetObject<AServerPawn>(Token);
			if (false == UNetObject::IsNetObject(Token))
			{
				std::shared_ptr<AServerPawn> NewServerPawn = GetWorld()->SpawnActor<AServerPawn>();
				ServerPawn = NewServerPawn.get();
				ServerPawn->SetControllOff();
				ServerPawn->InitNetObject(_Packet->GetObjectToken(), _Packet->GetSessionToken());
			}
			ServerPawn->SetActorLocation(_Packet->GetPosition());
		});
}

UServerEditor::UServerEditor()
{
}

UServerEditor::~UServerEditor()
{
	UserAccessPacket = nullptr;
}