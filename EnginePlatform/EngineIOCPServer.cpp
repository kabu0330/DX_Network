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
    std::cout << "[Server] ���� ���� ����" << std::endl;

    CreateIOCP();
    std::cout << "[Server] IOCP ����" << std::endl;

    GetAcceptEx();

    SetWorkerThread();

    SessionToken = SessionTokenCreator++;
}

void UEngineIOCPServer::CreateIOCPSockets()
{
    ListenSocket = CreateSocket(ENetworkType::IOCP);
    if (INVALID_SOCKET == ListenSocket)
    {
        MSGASSERT("���� ���� ������ �����߽��ϴ�.");
        return;
    }

    AcceptSocket = CreateSocket(ENetworkType::IOCP);
    if (INVALID_SOCKET == AcceptSocket)
    {
        MSGASSERT("���� ���� ������ �����߽��ϴ�.");
        return; 
    }
}

void UEngineIOCPServer::OpenListenServer()
{
    SOCKADDR_IN Address = CreateAddress(IPAddress, Port);
    if (false == Bind(ListenSocket, Address))
    {
        MSGASSERT("�ּҿ� ���� ���ε忡 �����߽��ϴ�.");
        return;
    }

    if (SOCKET_ERROR == listen(ListenSocket, UPlatformData::BackLog))
    {
        MSGASSERT("���� ���� ���¿� �����߽��ϴ�.");
        return;
    }
}

void UEngineIOCPServer::CreateIOCP()
{
    AcceptPort = CreateIoCompletionPort((HANDLE)ListenSocket, nullptr, 0, 0);
    if (nullptr == AcceptPort)
    {
        MSGASSERT("AcceptPort ���� ����");
        return;
    }

    SendRecvPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    if (nullptr == SendRecvPort)
    {
        MSGASSERT("Send/Recv Port ���� ����");
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
        MSGASSERT("AcceptEx �Լ� ������ ȹ�� �����߽��ϴ�.");
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



