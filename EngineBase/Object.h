#pragma once
#include <string>
#include <memory>
#include "EngineSerializer.h"

// 설명 : 이름 설정, 액티브 유무, 디버그 기능 지원
// std::enable_shared_from_this<UObject>
class UObject : public std::enable_shared_from_this<UObject>, public ISerializeObject
{
public:
	ENGINEAPI UObject();
	ENGINEAPI virtual ~UObject(); // 가상함수 테이블 생성

	template<typename ChildPtrType>
	std::shared_ptr<ChildPtrType> GetThis()
	{
		return std::static_pointer_cast<ChildPtrType>(shared_from_this());
	}

	std::string GetName() const
	{
		return Name;
	}

	std::string_view GetNameView() const
	{
		return Name.c_str();
	}

	virtual void SetName(std::string_view _Name)
	{
		Name = _Name.data();
	}

	virtual bool IsActive()
	{
		return bIsActive && false == bIsDestroy;
	}

	virtual bool IsDestroy()
	{
		return bIsDestroy;
	}

	void Destroy(float _Time = 0.0f)
	{
		DeathTime = _Time;

		if (0.0f < _Time)
		{
			bIsDeathTimeReached = true;
			return;
		}

		bIsDestroy = true;
	}

	virtual void ReleaseTimeCheck(float _DeltaTime)
	{
		if (false == bIsDeathTimeReached)
		{
			return;
		}

		CurDeathTime += _DeltaTime;

		if (DeathTime <= CurDeathTime)
		{
			bIsDestroy = true;
		}
	}

	virtual void ReleaseCheck(float _DeltaTime)
	{

	}

	// 모든 기능 정지, 외부에서 다른 객체가 풀어줘야 한다.
	void SetActive(bool _IsActive)
	{
		bIsActive = _IsActive;
	}

	void SetActiveSwitch()
	{
		bIsActive = !bIsActive;
	}

	bool IsDebug()
	{
		return bIsDebug;
	}

	void DebugOn()
	{
		bIsDebug = true;
	}

	void DebugOff()
	{
		bIsDebug = false;
	}

	void DebugSwitch()
	{
		bIsDebug = !bIsDebug;
	}

	int GetOrder()
	{
		return Order;
	}

	virtual void SetOrder(int _Order)
	{
		Order = _Order;
	}

	// 수정 가능
	bool& GetIsActiveValueRef()
	{
		return bIsActive;
	}

protected:

private:
	int Order = 0;

	bool bIsDestroy = false;
	bool bIsActive = true;

	bool bIsDeathTimeReached = false;
	float DeathTime = 0.0f;
	float CurDeathTime = 0.0f;

	std::string Name;

	bool bIsDebug = false;


private:
	// delete Function
	UObject(const UObject& _Other) = delete;
	UObject(UObject&& _Other) noexcept = delete;
	UObject& operator=(const UObject& _Other) = delete;
	UObject& operator=(UObject&& _Other) noexcept = delete;
};

