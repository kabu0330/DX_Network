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

// EngineCore���� ThreadPool �޾ƿ���
ENGINEAPI void RegisterTaskQueue(ITaskQueue* _Queue); 

// �����忡 �۾� ���
ENGINEAPI void WorkQueue(std::function<void()> _Work);

