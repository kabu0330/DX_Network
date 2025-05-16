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
	if (true == ThreadInst.joinable())
	{
		return false;
	}

	if (nullptr == _Function)
	{
		MSGASSERT("함수를 세팅하지 않은 쓰레드는 start 할수 없습니다.");
		return false;
	}

	Name = _Name;
	ThreadFunction = _Function;
	ThreadInst = std::thread(std::bind(ThreadBaseFunction, this));
	return true;
}

void UEngineThread::ThreadBaseFunction(UEngineThread* _Thread)
{
	// 스레드 작업의 진입점
	std::wstring WName = UEngineString::AnsiToUnicode(_Thread->Name);
	SetThreadDescription(GetCurrentThread(), WName.c_str());

	_Thread->ThreadFunction(); // Start()에서 넘겨준 함수 호출

}

void UEngineThread::Join()
{
	if (true == ThreadInst.joinable()) // 자기 자신을 Join하지 않도록 예외처리
	{
		ThreadInst.join();
	}
}
