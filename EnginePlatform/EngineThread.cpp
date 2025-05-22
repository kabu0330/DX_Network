#include "PreCompile.h"
#include "EngineThread.h"
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>

UEngineThread::UEngineThread()
{
}

UEngineThread::~UEngineThread()
{
	Join();
}

bool UEngineThread::Start(std::string _Name, std::function<void()> _Function)
{
	if (nullptr != ThreadHandle)
	{
		return false;
	}

	Name = std::move(_Name);
	ThreadFunction = std::move(_Function);

	unsigned threadID = 0;
	ThreadHandle = reinterpret_cast<HANDLE>(
		::_beginthreadex(
			nullptr,
			0,
			ThreadEntry,
			this,
			0,
			&threadID
		)
	);

	if (nullptr == ThreadHandle)
	{
		return false;
	}


#if defined(_MSC_VER) && defined(_DEBUG)
	// 디버거에 표시할 스레드 이름 설정
	SetThreadName(::GetThreadId(ThreadHandle), Name.c_str());
#endif

	return true;

	//if (true == ThreadInst.joinable())
	//{
	//	return false;
	//}

	//if (nullptr == _Function)
	//{
	//	MSGASSERT("함수를 세팅하지 않은 쓰레드는 start 할수 없습니다.");
	//	return false;
	//}

	//Name = _Name;
	//ThreadFunction = _Function;
	//ThreadInst = std::thread(std::bind(ThreadBaseFunction, this));
	//return true;
}

unsigned __stdcall UEngineThread::ThreadEntry(void* _Param)
{
	UEngineThread* Self = static_cast<UEngineThread*>(_Param);
	//std::wstring WName = UEngineString::AnsiToUnicode(Self->Name);
	//SetThreadDescription(GetCurrentThread(), WName.c_str());

	Self->ThreadFunction();

	::_endthreadex(0); // 내부 정리
	return 0;
}

//void UEngineThread::ThreadBaseFunction(UEngineThread* _Thread)
//{
//	// 스레드 작업의 진입점
//	std::wstring WName = UEngineString::AnsiToUnicode(_Thread->Name);
//	SetThreadDescription(GetCurrentThread(), WName.c_str());
//
//	_Thread->ThreadFunction(); // Start()에서 넘겨준 함수 호출
//}

void UEngineThread::Join()
{
	if (nullptr != ThreadHandle)
	{
		::WaitForSingleObject(ThreadHandle, INFINITE);
		::CloseHandle(ThreadHandle);
		ThreadHandle = nullptr;
	}

	ThreadFunction = nullptr;
	//if (true == ThreadInst.joinable()) // 자기 자신을 Join하지 않도록 예외처리
	//{
	//	ThreadInst.join();
	//}
}

void UEngineThread::SetThreadNameDynamic(const std::string& _ThreadName)
{
	SetThreadName(::GetCurrentThreadId(), _ThreadName.c_str());
}

