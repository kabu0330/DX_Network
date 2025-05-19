#include "PreCompile.h"
#include "TaskQueue.h"

static ITaskQueue* GTaskQueue = nullptr;

void WorkQueue(std::function<void()> _Work)
{
	if (nullptr != GTaskQueue)
	{
		GTaskQueue->WorkQueue(std::move(_Work));
	}
}

void RegisterTaskQueue(ITaskQueue* _Queue)
{
	GTaskQueue = _Queue;
}