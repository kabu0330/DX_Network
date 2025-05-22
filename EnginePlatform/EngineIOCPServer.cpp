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
    std::cout << "[Server] 리슨 서버 오픈" << std::endl;

    CreateIOCP();
    std::cout << "[Server] IOCP 생성" << std::endl;

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

    if (SOCKET_ERROR == listen(ListenSocket, UNetData::BackLog))
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
        MSGASSERT("CallAcceptEx 함수 포인터 획득 실패했습니다.");
        return;
    }
}

void UEngineIOCPServer::CallAcceptEx()
{
    DWORD DwBytes = 0;
    FnAccetEx(ListenSocket, AcceptSocket, UNetData::AcceptBuffer, 0, UNetData::SockAddrSize, UNetData::SockAddrSize, &DwBytes, &AcceptOverlapped); // 비동기 클라 접속 수락 함수
    std::cout << "[Server] 비동기 접속 대기" << std::endl;
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
            std::cout << "[Server] 클라이언트 접속 실패 알림" << std::endl;
            continue;
        }
        
        // 최초 통신
        if (&AcceptOverlapped == OverlappedPtr)
        {
            SOCKET ClientSocket = AcceptSocket;

            // 1. 향후 통신은 SendRecvPort Queue에 저장
            CreateIoCompletionPort((HANDLE)ClientSocket, SendRecvPort, 0, 0);

            // 2. 클라이언트 소켓 등록, 이름은 다음 통신에서
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



