#pragma once

//#include <thread>
#include <functional>
#include <process.h> 

// 설명 : 
class UEngineThread
{
public:
	ENGINEAPI UEngineThread();
	ENGINEAPI virtual ~UEngineThread();

	// delete Function
	UEngineThread(const UEngineThread& _Other) = delete;
	UEngineThread(UEngineThread&& _Other) noexcept = delete;
	UEngineThread& operator=(const UEngineThread& _Other) = delete;
	UEngineThread& operator=(UEngineThread&& _Other) noexcept = delete;

	ENGINEAPI bool Start(std::string _Name, std::function<void()> _Function);
	ENGINEAPI void Join();

protected:

private:
	std::string Name;
	//std::thread ThreadInst;
	std::function<void()> ThreadFunction;

	HANDLE ThreadHandle = nullptr;
	// CRT 스레드 엔트리 포인트
	static unsigned __stdcall ThreadEntry(void* _Param);

	static void ThreadBaseFunction(UEngineThread* _Thread);
};

