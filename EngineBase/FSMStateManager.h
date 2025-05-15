#pragma once
#include <functional>
#include <map>
#include "EngineDebug.h"

// 설명 :
class UFSMStateManager
{
public:
	UFSMStateManager()	{	}
	~UFSMStateManager()	{	}

	class FSMState
	{
	public:
		std::function<void()> StartFunction = nullptr;
		std::function<void(float)> UpdateFunction = nullptr;
		std::function<void()> EndFunction = nullptr;
	};

	template<typename EnumType>
	void CreateState(EnumType _Key, std::function<void(float)> _UpdateFunction, std::function<void()> _Start = nullptr)
	{
		CreateState(static_cast<int>(_Key), _UpdateFunction, _Start);
	}

	void CreateState(int _Key, std::function<void(float)> _UpdateFunction, std::function<void()> _Start = nullptr)
	{
		if (true == States.contains(_Key))
		{
			MSGASSERT("이미 존재하는 Key입니다. 중복을 허용하지 않습니다.");
			return;
		}

		States[_Key].UpdateFunction = _UpdateFunction;
		States[_Key].StartFunction = _Start;
	}

	void Update(float _DeltaTime)
	{
		if (nullptr == CurState)
		{
			MSGASSERT("상태가 지정되지 않은 스테이트머신 입니다. CreateState()로 상태를 추가해주세요.");
			return;
		}

		CurState->UpdateFunction(_DeltaTime);
	}

	template<typename EnumType>
	void ChangeState(EnumType _Key)
	{
		ChangeState(static_cast<int>(_Key));
	}

	void ChangeState(int _Key)
	{
		if (false == States.contains(_Key))
		{
			MSGASSERT("존재하지 않는 상태입니다. CreateState()로 상태를 추가해주세요.");
			return;
		}

		PrevState = CurState;
		CurState = &States[_Key];
		if (nullptr != CurState->StartFunction)
		{
			CurState->StartFunction();
		}
	}

	void ChangePrevState()
	{
		CurState = PrevState;
		if (nullptr != CurState->StartFunction)
		{
			CurState->StartFunction();
		}
	}

protected:

private:
	FSMState* CurState = nullptr;
	FSMState* PrevState = nullptr;
	std::map<int, FSMState> States;


private:
	// delete Function
	UFSMStateManager(const UFSMStateManager& _Other) = delete;
	UFSMStateManager(UFSMStateManager&& _Other) noexcept = delete;
	UFSMStateManager& operator=(const UFSMStateManager& _Other) = delete;
	UFSMStateManager& operator=(UFSMStateManager&& _Other) noexcept = delete;
};

