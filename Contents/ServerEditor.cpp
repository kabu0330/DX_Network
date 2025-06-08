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

		//if (true == ImGui::Button(UEngineString::AnsiToUTF8("IOCP방만들기").c_str()))
		//{
		//	GetWorld()->GetGameMode()->StartIOCPServer(Port);
		//	IOCPServer = GetWorld()->GetGameMode()->GetServer<UEngineIOCPServer>();
		//	CreateServer(IOCPServer);
		//	GEngine->GetMainWindow().SetWindowTitle("IOCP Server");
		//}

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
	int ObjectToken = _Net->CreateObjectToken();
	Pawn->CreateNetObjectSetting(ObjectToken, _Net->GetSessionToken());

	// 다른 플레이어 생성 및 위치 동기화
	_Net->GetDispatcher().AddHandler<UObjectUpdatePacket>(static_cast<int>(EContentsPacketType::OBJECT_UPDATE), 
		[this, _Net](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			AServerPawn* ServerPawn = UNetObject::GetConvertNetObject<AServerPawn>(Token);
			if (false == UNetObject::IsNetObject(Token)) // 최초 접속 이라면
			{
				std::shared_ptr<AServerPawn> NewServerPawn = GetWorld()->SpawnActor<AServerPawn>();
				ServerPawn = NewServerPawn.get();
				ServerPawn->SetControllOff(); // 서버가 클라의 통제권을 갖지 않는다.
				ServerPawn->CreateNetObjectSetting(_Packet->GetObjectToken(), _Packet->GetSessionToken());
			}

			ServerPawn->SetActorLocation(_Packet->GetPosition());

			_Net->SendPacket(_Packet); // 다른 클라에게도 전송
		});
}

void UServerEditor::CreateNetObjectSetting(std::shared_ptr<UUserAccessPacket> _Packet)
{
	// 이미 기존에 만들어진 자신에게 오브젝트 토큰 + 세션 토큰을 저장
	UserAccessPacket = _Packet;
	AServerPawn* Pawn = GetWorld()->GetMainPawn<AServerPawn>(); 
	Pawn->CreateNetObjectSetting(UserAccessPacket->GetObjectToken(), UserAccessPacket->GetSessionToken());
}

void UServerEditor::Connect(std::shared_ptr<UEngineClient> _Net)
{
	_Net->SetUserAccessFunction(std::bind(&UServerEditor::CreateNetObjectSetting, this, std::placeholders::_1));

	_Net->GetDispatcher().AddHandler<UObjectUpdatePacket>(static_cast<int>(EContentsPacketType::OBJECT_UPDATE), 
		[this](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			AServerPawn* ServerPawn = UNetObject::GetConvertNetObject<AServerPawn>(Token);
			if (false == UNetObject::IsNetObject(Token)) // 최초 접속이라면 클라 화면에도 스폰시키고
			{
				std::shared_ptr<AServerPawn> NewServerPawn = GetWorld()->SpawnActor<AServerPawn>();
				ServerPawn = NewServerPawn.get();
				ServerPawn->SetControllOff();
				ServerPawn->CreateNetObjectSetting(_Packet->GetObjectToken(), _Packet->GetSessionToken());
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