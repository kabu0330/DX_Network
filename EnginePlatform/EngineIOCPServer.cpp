#include "PreCompile.h"
#include "EngineIOCPServer.h"

UEngineIOCPServer::UEngineIOCPServer()
{
}

UEngineIOCPServer::~UEngineIOCPServer()
{
}

void UEngineIOCPServer::OpenIOCPServer(int _Port)
{
	StartNetwork();

	Port = _Port;
	
    IPAddress = "0.0.0.0";

    CreateIOCPSockets();

    OpenListenServer();
    std::cout << "[Server] 리슨 서버 오픈" << std::endl;

    CreateIOCP();
    std::cout << "[Server] IOCP 생성" << std::endl;

    GetAcceptEx();

    SetWorkerThread();

    SessionToken = SessionTokenCreator++;
}

void UEngineIOCPServer::CreateIOCPSockets()
{
    ListenSocket = CreateSocket(ENetworkType::IOCP);
    if (INVALID_SOCKET == ListenSocket)
    {
        MSGASSERT("리슨 소켓 생성에 실패했습니다.");
        return;
    }

    AcceptSocket = CreateSocket(ENetworkType::IOCP);
    if (INVALID_SOCKET == AcceptSocket)
    {
        MSGASSERT("접속 소켓 생성에 실패했습니다.");
        return; 
    }
}

void UEngineIOCPServer::OpenListenServer()
{
    SOCKADDR_IN Address = CreateAddress(IPAddress, Port);
    if (false == Bind(ListenSocket, Address))
    {
        MSGASSERT("주소와 세션 바인드에 실패했습니다.");
        return;
    }

    if (SOCKET_ERROR == listen(ListenSocket, UPlatformData::BackLog))
    {
        MSGASSERT("리슨 서버 오픈에 실패했습니다.");
        return;
    }
}

void UEngineIOCPServer::CreateIOCP()
{
    AcceptPort = CreateIoCompletionPort((HANDLE)ListenSocket, nullptr, 0, 0);
    if (nullptr == AcceptPort)
    {
        MSGASSERT("AcceptPort 생성 실패");
        return;
    }

    SendRecvPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    if (nullptr == SendRecvPort)
    {
        MSGASSERT("Send/Recv Port 생성 실패");
        return;
    }
}

void UEngineIOCPServer::GetAcceptEx()
{
    GUID GuidAcceptEx = WSAID_ACCEPTEX;
    DWORD DwBytes = 0;
    WSAIoctl(ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx), &FnAccetEx, sizeof(FnAccetEx), &DwBytes, nullptr, nullptr);
    if (nullptr == FnAccetEx)
    {
        MSGASSERT("AcceptEx 함수 포인터 획득 실패했습니다.");
        return;
    }
}

void UEngineIOCPServer::SetWorkerThread()
{
    ConnectAcceptThread.Start("ConnectAcceptThread", std::bind(&UEngineServer::AcceptThreadFunction, this, ListenSocket));

}

void UEngineIOCPServer::Release()
{
	UEngineNetwork::Release();

	bIsActive = false;
}



