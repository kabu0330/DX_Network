#pragma once
#include <string>
#include <memory>
#include "EngineSerializer.h"

// ���� : �̸� ����, ��Ƽ�� ����, ����� ��� ����
// std::enable_shared_from_this<UObject>
class UObject : public std::enable_shared_from_this<UObject>, public ISerializeObject
{
public:
	ENGINEAPI UObject();
	ENGINEAPI virtual ~UObject(); // �����Լ� ���̺� ����

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

	// ��� ��� ����, �ܺο��� �ٸ� ��ü�� Ǯ����� �Ѵ�.
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

	// ���� ����
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

