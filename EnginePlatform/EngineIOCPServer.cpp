#include "PreCompile.h"
#include "EngineIOCPServer.h"
#include "TaskQueue.h"

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
    CallAcceptEx();

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

    if (SOCKET_ERROR == listen(ListenSocket, UNetData::BackLog))
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
        MSGASSERT("CallAcceptEx �Լ� ������ ȹ�� �����߽��ϴ�.");
        return;
    }
}

void UEngineIOCPServer::CallAcceptEx()
{
    DWORD DwBytes = 0;
    FnAccetEx(ListenSocket, AcceptSocket, UNetData::AcceptBuffer, 0, UNetData::SockAddrSize, UNetData::SockAddrSize, &DwBytes, &AcceptOverlapped); // �񵿱� Ŭ�� ���� ���� �Լ�
    std::cout << "[Server] �񵿱� ���� ���" << std::endl;
}

void UEngineIOCPServer::SetWorkerThread()
{
    WorkQueue(std::bind(&UEngineIOCPServer::AcceptThread, this, this));

}

void UEngineIOCPServer::AcceptThread(UEngineIOCPServer* _Server)
{
    DWORD BytesTransferred = 0;
    ULONG_PTR Key = 0;
    OVERLAPPED* OverlappedPtr = nullptr;
    DWORD dwBytes = 0;
    while (true)
    {
        BOOL bIsOK = GetQueuedCompletionStatus(AcceptPort, &BytesTransferred, &Key, &OverlappedPtr, INFINITE);
        if (false == bIsOK || nullptr == OverlappedPtr)
        {
            std::cout << "[Server] Ŭ���̾�Ʈ ���� ���� �˸�" << std::endl;
            continue;
        }
        
        // ���� ���
        if (&AcceptOverlapped == OverlappedPtr)
        {
            SOCKET ClientSocket = AcceptSocket;

            // 1. ���� ����� SendRecvPort Queue�� ����
            CreateIoCompletionPort((HANDLE)ClientSocket, SendRecvPort, 0, 0);

            // 2. Ŭ���̾�Ʈ ���� ���, �̸��� ���� ��ſ���
            EnterCriticalSection(&Lock);
            AllClients[ClientSocket] = FClientData();
            LeaveCriticalSection(&Lock);

            std::shared_ptr<char[]> RecvBuffer = std::make_shared<char[]>(UNetData::BufferSize);
            


        }

    }
}

void UEngineIOCPServer::Release()
{
	UEngineNetwork::Release();

	bIsActive = false;
}



