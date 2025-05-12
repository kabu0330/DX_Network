#include "PreCompile.h"
#include "EngineWorkThreadPool.h"

class UWork
{
public:
	std::function<void()> Function;
};

void UEngineWorkThreadPool::Initialize(std::string_view ThreadName /*= "WorkThread"*/, int Count /*= 0*/)
{
	ThreadCount = Count;

	if (0 >= Count)
	{
		SYSTEM_INFO Info;
		GetSystemInfo(&Info);
		ThreadCount = Info.dwNumberOfProcessors; // 내 컴퓨터의 코어 개수
	}

	RunningCount = ThreadCount;

	// CreateIoCompletionPort 윈도우야 나 이제부터 쓰레드 관리할래 
	// 나이제 IOCP 시작하고 싶어
	// IOCP 관리핸들 만들어줘.
	IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	// 파일 통합관리를 하고 싶다고 해도 IOCP 관리핸들 만들어줘.
	/*FILE* FILE;
	SOCKET Socket;
	CreateIoCompletionPort(IOCPHandle, Socket, 0, 0);*/

	// 서버통신을 하면 이걸 사용해서 이 파일을 관리하고 
	// CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	if (nullptr == IOCPHandle)
	{
		MSGASSERT("IOCP 핸들 생성에 실패했습니다.");
		return;
	}

	Threads.resize(ThreadCount);
	for (size_t i = 0; i < ThreadCount; i++)
	{
		Threads[i] = std::make_shared<UEngineThread>();

		Threads[i]->Start(std::string(ThreadName) + std::to_string(i), std::bind(ThreadQueueFunction, IOCPHandle, this));
	}
}

void UEngineWorkThreadPool::WorkQueue(std::function<void()> _Work)
{
	UWork* NewWork = new UWork();
	NewWork->Function = _Work;

	// 쓰레드풀을 만들때는 이게 가장 핵심 함수이다.
	// IOCP 쓰레드중 가장 적절한 쓰레드를 깨워라.
	PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(EThreadStatus::Work), reinterpret_cast<ULONG_PTR>(NewWork), nullptr);
}

void UEngineWorkThreadPool::ThreadQueueFunction(HANDLE _IOCPHandle, UEngineWorkThreadPool* _JobQueue)
{
	/*_Out_ LPDWORD lpNumberOfBytesTransferred,
	_Out_ PULONG_PTR lpCompletionKey,
	_Out_ LPOVERLAPPED* lpOverlapped,
	_In_ DWORD dwMilliseconds*/

	// 이게 만약 파일입출력이면 받은 바이트 크기
	unsigned long Byte = 0;
	unsigned __int64 Ptr;

	// 가장 쓸모가 없다.
	// 지금 파일이나 소켓을 얼마나 읽어고 등의 정보가 
	LPOVERLAPPED OverPtr = nullptr;

	// 쓰레드를 죽이지 않고 계속 살려두기 위해서 while걸었다.
	while (_JobQueue->IsWork)
	{
		// 일이 있을때까지 잔다.
		// _IOCPHandle <= 주체 일이있으면 깨워 
		// 마지막 인자는 1000의 시간이 지나면 그냥 아무일 없어도 일어나
		// GetQueuedCompletionStatus(_IOCPHandle, &Byte, &Ptr, &OverPtr, 1000);

		GetQueuedCompletionStatus(_IOCPHandle, &Byte, &Ptr, &OverPtr, INFINITE);

		if (-1 == Byte)
		{
			break;
		}

		if (-2 == Byte)
		{
			UWork* Work = reinterpret_cast<UWork*>(Ptr);

			if (nullptr != Work)
			{
				Work->Function();
			}

			delete Work;
		}
	}

	_JobQueue->RunningCount -= 1;
}

UEngineWorkThreadPool::UEngineWorkThreadPool()
{
}

UEngineWorkThreadPool::~UEngineWorkThreadPool()
{
	IsWork = false;

	// 쓰레드를 종료하라는 숫자가 -1
	while (0 < RunningCount)
	{
		PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(EThreadStatus::Destroy), 0, nullptr);
	}
}