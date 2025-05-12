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
	// 이 함수가 쓰레드로 실행되는 것이다.

	std::wstring WName = UEngineString::AnsiToUnicode(_Thread->Name);
	SetThreadDescription(GetCurrentThread(), WName.c_str());

	_Thread->ThreadFunction();

	// 쓰레드로 실행된 함수 내에서만 이름을 바꿀 수가 있다.
}

void UEngineThread::Join()
{
	ThreadInst.join();
}
