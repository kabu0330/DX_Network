#include "PreCompile.h"
#include "TetrisPlayEditor.h"
#include "ServerPawn.h"
#include <EnginePlatform/EngineDispatcher.h>
#include "ContentPacket.h"
#include "ServerGameMode.h"
#include <EngineCore/PlayerController.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineCore/EngineCore.h>
#include "Tetromino.h"
#include "GameField.h"
#include "GlobalData.h"

void UTetrisPlayEditor::OnGUI(float _DeltaTime)
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
	}
}

void UTetrisPlayEditor::CreateServer(std::shared_ptr<UEngineServer> _Net)
{
	ATetromino* Pawn = GetWorld()->GetMainPawn<ATetromino>();
	int ObjectToken = _Net->CreateObjectToken();
	Pawn->CreateNetObjectSetting(ObjectToken, _Net->GetSessionToken());
	Pawn->GetPlayerController()->SwitchControlled();

	// 다른 플레이어 생성 및 위치 동기화
	_Net->GetDispatcher().AddHandler<UObjectUpdatePacket>(static_cast<int>(EContentsPacketType::OBJECT_UPDATE), 
		[this, _Net](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			AServerPawn* ServerPawn = UNetObject::GetConvertNetObject<AServerPawn>(Token);
			if (false == UNetObject::IsNetObject(Token)) // 최초 접속 이라면
			{
				std::shared_ptr<AServerPawn> NewServerPawn = GetWorld()->SpawnActor<ATetromino>();
				ServerPawn = NewServerPawn.get();
				ServerPawn->SetControllOff(); // 서버가 클라의 통제권을 갖지 않는다.
				ServerPawn->CreateNetObjectSetting(_Packet->GetObjectToken(), _Packet->GetSessionToken());

				// 테트리스 맵 생성
				auto SyncFunction = [this](std::shared_ptr<USpawnPacket> _Packet)
					{
						_Packet->SetSubclassType(ETypeInfo::AGameField);
					};

				GetWorld()->GetMainPawn<ATetromino>()->GetNetHandler()->SendPacket<USpawnPacket>(SyncFunction, true);
			}

			// 처리
			ServerPawn->SetActorLocation(_Packet->GetPosition());
			ServerPawn->SetActorRotation(_Packet->GetRotation());

			_Net->SendPacket(_Packet); // 다른 클라에게도 전송
		});

	_Net->GetDispatcher().AddHandler<UMinoUpdatePacket>(static_cast<int>(EContentsPacketType::MINO_UPDATE),
		[this, _Net](std::shared_ptr<UMinoUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			ATetromino* ServerPawn = UNetObject::GetConvertNetObject<ATetromino>(Token);

			ServerPawn->SetType(_Packet->GetMinoType());

			_Net->SendPacket(_Packet);
		});

	_Net->GetDispatcher().AddHandler<USpawnPacket>(static_cast<int>(EContentsPacketType::SPAWN_ACTOR),
		[this, _Net](std::shared_ptr<USpawnPacket> _Packet)
		{
			// 1. 타입 설정
			AServerActor* NetActor = UGlobalData::GetSpawnActor(_Packet->GetSpawnObjectToken(), GetWorld());
			if (nullptr == NetActor)
			{
				MSGASSERT("스폰 요청을 한 액터가 AServerActor를 상속하지 않았습니다.");
				return;
			}

			// 2. 오브젝트 토큰 부여
			int ObjectToken = _Net->CreateObjectToken(); // 서버만이 할 수 있는 작업
			_Packet->SetSpawnObjectToken(ObjectToken);
			NetActor->CreateNetObjectSetting(ObjectToken, _Net->GetSessionToken());

			// 3. 초기 위치 설정
			FVector Pos = _Packet->GetInitPos();
			NetActor->SetActorLocation({ Pos.X + 300.0f, Pos.Y });

			_Net->SendPacket(_Packet);
		});
}

void UTetrisPlayEditor::Connect(std::shared_ptr<UEngineClient> _Net)
{
	_Net->SetUserAccessFunction([this](std::shared_ptr<UUserAccessPacket> _Packet)
		{
			UserAccessPacket = _Packet;
			ATetromino* Pawn = GetWorld()->GetMainPawn<ATetromino>();
			Pawn->CreateNetObjectSetting(UserAccessPacket->GetObjectToken(), UserAccessPacket->GetSessionToken());
			Pawn->GetPlayerController()->SwitchControlled();
		});

	_Net->GetDispatcher().AddHandler<USpawnPacket>(static_cast<int>(EContentsPacketType::SPAWN_ACTOR),
		[this, _Net](std::shared_ptr<USpawnPacket> _Packet)
		{
			// 1. 타입 설정
			AServerActor* NetActor = UGlobalData::GetSpawnActor(_Packet->GetSpawnObjectToken(), GetWorld());
			if (nullptr == NetActor)
			{
				MSGASSERT("스폰 요청을 한 액터가 AServerActor를 상속하지 않았습니다.");
				return;
			}

			// 2. 오브젝트 토큰 부여
			NetActor->CreateNetObjectSetting(_Packet->GetSpawnObjectToken(), _Net->GetSessionToken());

			// 3. 초기 위치 설정
			FVector Pos = _Packet->GetInitPos();
			NetActor->SetActorLocation({ Pos.X + 300.0f, Pos.Y });
		});

	_Net->GetDispatcher().AddHandler<UObjectUpdatePacket>(static_cast<int>(EContentsPacketType::OBJECT_UPDATE),
		[this, _Net](std::shared_ptr<UObjectUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			AServerPawn* ServerPawn = UNetObject::GetConvertNetObject<AServerPawn>(Token);
			if (false == UNetObject::IsNetObject(Token)) // 최초 접속이라면 클라 화면에도 스폰시키고
			{
				std::shared_ptr<AServerPawn> NewServerPawn = GetWorld()->SpawnActor<ATetromino>();
				ServerPawn = NewServerPawn.get();
				ServerPawn->SetControllOff();
				ServerPawn->CreateNetObjectSetting(_Packet->GetObjectToken(), _Packet->GetSessionToken());

				//// 테트리스 맵 생성
				//auto SyncFunction = [this](std::shared_ptr<USpawnPacket> _Packet)
				//	{
				//		_Packet->SetSubclassType(ETypeInfo::AGameField);
				//	};

				////GetWorld()->GetMainPawn<ATetromino>()->GetNetHandler()->SendPacket<USpawnPacket>(SyncFunction);
			}

			ServerPawn->SetActorLocation(_Packet->GetPosition());
			ServerPawn->SetActorRotation(_Packet->GetRotation());
		});

	_Net->GetDispatcher().AddHandler<UMinoUpdatePacket>(static_cast<int>(EContentsPacketType::MINO_UPDATE),
		[this, _Net](std::shared_ptr<UMinoUpdatePacket> _Packet)
		{
			int Token = _Packet->GetObjectToken();
			ATetromino* ServerPawn = UNetObject::GetConvertNetObject<ATetromino>(Token);

			ServerPawn->SetType(_Packet->GetMinoType());
		});
}

UTetrisPlayEditor::UTetrisPlayEditor()
{
}

UTetrisPlayEditor::~UTetrisPlayEditor()
{
	UserAccessPacket = nullptr;
}