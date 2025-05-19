#pragma once
#include <functional>
#include "EngineBase/EngineDefine.h"

// ���� : ThreadPool�� ������������ ����ϱ� ���� �������̽�
class ITaskQueue
{
public:
	virtual void WorkQueue(std::function<void()> _Work) = 0;
	virtual ~ITaskQueue() = default;
};

ENGINEAPI void WorkQueue(std::function<void()> _Work);
ENGINEAPI void RegisterTaskQueue(ITaskQueue* _Queue);

