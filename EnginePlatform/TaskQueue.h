#pragma once
#include <functional>
#include "EngineBase/EngineDefine.h"

// 설명 : ThreadPool을 상위계층에서 사용하기 위한 인터페이스
class ITaskQueue
{
public:
	virtual void WorkQueue(std::function<void()> _Work) = 0;
	virtual ~ITaskQueue() = default;
};

ENGINEAPI void WorkQueue(std::function<void()> _Work);
ENGINEAPI void RegisterTaskQueue(ITaskQueue* _Queue);

