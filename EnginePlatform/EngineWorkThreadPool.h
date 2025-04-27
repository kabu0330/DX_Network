#pragma once
#include <functional>
#include "EngineThread.h"
#include <atomic>

enum class EThreadStatus
{
	Work = -2,
	Destroy = -1,
};

// 설명 :
class UEngineWorkThreadPool
{
	// 
public:
	// constrcuter destructer
	// 0개를 넣으면 알아서 코어개수를 파악해서 쓰레드를 만들어준다.
	ENGINEAPI UEngineWorkThreadPool();
	ENGINEAPI ~UEngineWorkThreadPool();

	// delete Function
	UEngineWorkThreadPool(const UEngineWorkThreadPool& _Other) = delete;
	UEngineWorkThreadPool(UEngineWorkThreadPool&& _Other) noexcept = delete;
	UEngineWorkThreadPool& operator=(const UEngineWorkThreadPool& _Other) = delete;
	UEngineWorkThreadPool& operator=(UEngineWorkThreadPool&& _Other) noexcept = delete;

	ENGINEAPI void Initialize(std::string_view ThreadName = "WorkThread", int Count = 0);

	ENGINEAPI void WorkQueue(std::function<void()> _Work);

protected:

private:
	std::atomic<int> RunningCount;
	std::atomic<bool> IsWork = true;

	HANDLE IOCPHandle = nullptr;

	int ThreadCount = 0;
	// 코어개수 * 2 + 1 => 
	std::vector<std::shared_ptr<UEngineThread>> Threads;

	static void ThreadQueueFunction(HANDLE _IOCPHandle, UEngineWorkThreadPool* _JobQueue);
};

