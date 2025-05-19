#include "PreCompile.h"
#include "EngineServer.h"

UEngineServer::UEngineServer()
{
}

UEngineServer::~UEngineServer()
{
    Release();
}

void UEngineServer::OpenServer(int _Port, int _BackLog)
{
    StartNetwork();

    Port = _Port;
    BackLog = _BackLog;

    IPAddress = "0.0.0.0";

    ListenSocket = CreateSocket(ENetworkType::TCP);
    SOCKADDR_IN Address = CreateAddress(IPAddress, _Port);
    if (false == Bind(ListenSocket, Address))
    {
        MSGASSERT("주소와 세션 바인드에 실패했습니다.");
        return;
    }

    if (SOCKET_ERROR == listen(ListenSocket, BackLog))
    {
        MSGASSERT("리슨 서버 오픈에 실패했습니다.");
        return;
    }

    std::cout << "[Server] 리슨 서버 오픈" << std::endl;

    ConnectAcceptThread.Start("ConnectAcceptThread", std::bind(&UEngineServer::AcceptThreadFunction, this, ListenSocket));

    SessionToken = SessionTokenCreator++;

}

void UEngineServer::AcceptThreadFunction(UEngineServer* _Server, SOCKET _ListenSocket)
{
    while (true == _Server->bIsActive)
    {
		fd_set ReadSet;
		FD_ZERO(&ReadSet);
		FD_SET(_ListenSocket, &ReadSet);

		timeval Timeout = { 1, 0 }; // 1초 체크
		int Result = select(0, &ReadSet, nullptr, nullptr, &Timeout);

		if (Result == 0)
		{
			continue; // 타임아웃, 루프 재진입
		}

		if (Result < 0)
		{
			break; // 소켓 에러
		}

		if (FD_ISSET(_ListenSocket, &ReadSet))
		{
			int AddressLength = sizeof(SOCKADDR_IN);
			SOCKADDR_IN ClientAddress;
			memset(&ClientAddress, 0, sizeof(ClientAddress));

			SOCKET ConnectSocket = accept(_ListenSocket, (sockaddr*)&ClientAddress, &AddressLength);
			if (ConnectSocket == INVALID_SOCKET)
			{
				std::cout << "[Server] 클라이언트 accept 실패" << std::endl;
				continue;
			}

			{
				std::lock_guard<std::mutex> Lock(_Server->UserLock);
				std::shared_ptr<UEngineThread> Thread = std::make_shared<UEngineThread>();

				UEngineSerializer Ser;

				std::shared_ptr<UUserAccessPacket> AcceptProtocol = std::make_shared<UUserAccessPacket>();
				AcceptProtocol->SetPacketType(EEnginePacketType::UserAccessPacket);
				AcceptProtocol->SetSessionToken(_Server->CreateSessionToken());
				AcceptProtocol->SetObjectToken(_Server->CreateObjectToken());

				AcceptProtocol->SerializePacket(Ser);

				// 서버는 세션토큰으로 클라이언트를 관리한다.
				_Server->AllUserSockets.insert({ AcceptProtocol->GetSessionToken(), ConnectSocket });

				send(ConnectSocket, Ser.GetDataBuffer(), Ser.GetWritePos(), 0);

				std::string Client = std::to_string(ConnectSocket);
				Thread->Start("User " + Client, std::bind(&UEngineNetwork::RecvTCPThreadFunction, _Server, ConnectSocket));

				_Server->AllUserThreads.push_back(Thread);
			}
		}
	}
}

void UEngineServer::SendPacket(UEngineProtocol* _Protocol)
{
    if (-1 == ListenSocket)
    {
        return;
    }

    {
        std::lock_guard<std::mutex> Lock(UserLock);
        UEngineSerializer Ser;
        _Protocol->SerializePacket(Ser);

        for (std::pair<const int, SOCKET>& Session : AllUserSockets)
        {
            if (Session.first == _Protocol->GetSessionToken())
            {
                continue; // 패킷을 서버로 보낸 대상은 제외하고 나머지 클라에게 패킷을 보내야한다.
            } 

            send(Session.second, Ser.GetDataBuffer(), Ser.GetWritePos(), 0);
        }
    }
}

int UEngineServer::CreateObjectToken()
{
    return ObjectTokenCreator++;
}

int UEngineServer::CreateSessionToken()
{
    return SessionTokenCreator++;
}

void UEngineServer::Release()
{
    UEngineNetwork::Release();

    bIsActive = false;

    {
        std::lock_guard<std::mutex> Lock(UserLock);
        for (std::pair<const int, SOCKET>& Session : AllUserSockets)
        {
            closesocket(Session.second);
        }

        AllUserSockets.clear();
    }

    if (0 != ListenSocket)
    {
        shutdown(ListenSocket, SD_BOTH);
        closesocket(ListenSocket);
        ListenSocket = 0;
    }

    ConnectAcceptThread.Join();

    for (std::shared_ptr<UEngineThread>& Thread : AllUserThreads)
    {
        if (nullptr != Thread)
        {
            Thread->Join();
        }
    }
    AllUserThreads.clear();

}

