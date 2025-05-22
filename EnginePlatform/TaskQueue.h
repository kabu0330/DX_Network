#pragma once
#include <functional>
#include "EngineBase/EngineDefine.h"
#include "EngineThread.h"

// 설명 : ThreadPool을 상위계층에서 사용하기 위한 인터페이스
class ITaskQueue
{
public:
	virtual void WorkQueue(std::function<void()> _Work) = 0;
	virtual ~ITaskQueue() = default;
};

// EngineCore에서 ThreadPool 받아오기
ENGINEAPI void RegisterTaskQueue(ITaskQueue* _Queue); 

// 스레드에 작업 명령
ENGINEAPI void WorkQueue(std::function<void()> _Work);

