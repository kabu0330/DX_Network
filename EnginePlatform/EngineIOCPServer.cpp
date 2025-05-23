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

    CreateListenSocket();

    OpenListenServer();
    std::cout << "[Server] 리슨 서버 오픈" << std::endl;

    CreateIOCP();
    std::cout << "[Server] IOCP 생성" << std::endl;

    GetAcceptEx();
    CreateAcceptSocket();
    CallAcceptEx();

    SetWorkThread();

    SessionToken = SessionTokenCreator++;
}

void UEngineIOCPServer::CreateListenSocket()
{
    ListenSocket = CreateSocket(ENetworkType::IOCP);
    if (INVALID_SOCKET == ListenSocket)
    {
        MSGASSERT("ListenSocket 생성에 실패했습니다.");
        return;
    }
}

void UEngineIOCPServer::CreateAcceptSocket()
{
    AcceptSocket = CreateSocket(ENetworkType::IOCP);
    if (INVALID_SOCKET == AcceptSocket)
    {
        MSGASSERT("AcceptSocket 생성에 실패했습니다.");
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
    ZeroMemory(&AcceptOverlapped, sizeof(OVERLAPPED));

    FnAccetEx(ListenSocket, AcceptSocket, UNetData::AcceptBuffer, 0, UNetData::SockAddrSize, UNetData::SockAddrSize, &DwBytes, &AcceptOverlapped); // 비동기 클라 접속 수락 함수

    std::cout << "[Server] 비동기 접속 대기" << std::endl;
}

void UEngineIOCPServer::SetWorkThread()
{
    WorkQueue(std::bind(&UEngineIOCPServer::AcceptThread, this, this));
}

void UEngineIOCPServer::AcceptThread(UEngineIOCPServer* _Server)
{
    DWORD BytesTransferred = 0;
    ULONG_PTR Key = 0;
    OVERLAPPED* OverlappedPtr = nullptr;
    DWORD dwBytes = 0;
    static int AcceptThreadNumber = 0;
    while (true == _Server->bIsActive)
    {
        UEngineThread::SetThreadNameDynamic("AcceptThread" + std::to_string(AcceptThreadNumber));
        ++AcceptThreadNumber;

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
            WSABUF WsaBuffer = {};
            WsaBuffer.buf = RecvBuffer.get();
            WsaBuffer.len = UNetData::BufferSize;

            std::shared_ptr<FOverlappedEx> RecvContext = std::make_shared<FOverlappedEx>();
            ZeroMemory(RecvContext.get(), sizeof(*RecvContext));

            RecvContext->Socket = ClientSocket;
            RecvContext->WsaBuf = WsaBuffer;
            RecvContext->Buffer = WsaBuffer.buf;
            RecvContext->OperationType = EOperationType::ACCEPT;
            
            ZeroMemory(&RecvContext->Overlapped, sizeof(OVERLAPPED));

            SetWSARecv(RecvBuffer.get(), RecvContext.get());
            
            // 다시 AcceptEx 요청
            CreateAcceptSocket();
            CallAcceptEx();
        }
    }
    --AcceptThreadNumber;
}

bool UEngineIOCPServer::SetWSARecv(char* _Buffer, FOverlappedEx* _Context)
{
    DWORD Flags = 0;
    int Result = WSARecv(_Context->Socket, &_Context->WsaBuf, 1, nullptr, &Flags, &_Context->Overlapped, nullptr);
    int Error = WSAGetLastError();
    if (SOCKET_ERROR == Result && ERROR_IO_PENDING != Error)
    {
        std::cerr << "WSA Recv Failed : " << Error << std::endl;
        closesocket(_Context->Socket);
        _Buffer = nullptr;
        _Context = nullptr;
        
        return false;
    }

    return true;
}

void UEngineIOCPServer::Release()
{
	UEngineNetwork::Release();

	bIsActive = false; // 종료 트리거

    // AcceptEx 취소
    CancelIoEx((HANDLE)ListenSocket, &AcceptOverlapped);

    // 종료 신호용 Completion Port 알림
    PostQueuedCompletionStatus(AcceptPort, 0, 0, nullptr);

    DWORD Bytes = 0;
    ULONG_PTR Key = 0;
    OVERLAPPED* Overlapped = nullptr;
    BOOL bIsDelay = GetQueuedCompletionStatus(AcceptPort, &Bytes, &Key, &Overlapped, 3000);
    
    closesocket(AcceptSocket);
    closesocket(ListenSocket);

    if (INVALID_HANDLE_VALUE != AcceptPort)
    {
        CloseHandle(AcceptPort);
    }
    if (INVALID_HANDLE_VALUE != SendRecvPort)
    {
        CloseHandle(SendRecvPort);
    }

    WSACleanup();
}



